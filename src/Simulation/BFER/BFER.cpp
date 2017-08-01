#include <cmath>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#ifdef ENABLE_MPI
#include "Module/Monitor/Standard/Monitor_reduction_mpi.hpp"
#endif

#include "Factory/Module/Monitor.hpp"
#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER<B,R,Q>
::BFER(const factory::BFER::parameters& params, Codec<B,Q> &codec)
: Simulation(),
  params(params),

  stop_terminal(false),

  codec(codec),

  barrier(params.n_threads),

  bit_rate((float)params.src->K / (float)params.pct->N),

  snr  (0.f),
  snr_s(0.f),
  snr_b(0.f),
  sigma(0.f),

  max_fra(0),

  monitor    (params.n_threads, nullptr),
  monitor_red(                  nullptr),
  dumper     (params.n_threads, nullptr),
  dumper_red (                  nullptr),
  terminal   (                  nullptr),

  durations(params.n_threads)
{
	if (params.n_threads < 1)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << params.n_threads << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (params.err_track_enable)
	{
		for (auto tid = 0; tid < params.n_threads; tid++)
			dumper[tid] = new Dumper(params.src->n_frames);

		std::vector<Dumper*> dumpers;
		for (auto tid = 0; tid < params.n_threads; tid++)
			dumpers.push_back(dumper[tid]);

		dumper_red = new Dumper_reduction(dumpers, params.src->n_frames);
	}

	for (auto tid = 0; tid < params.n_threads; tid++)
		this->monitor[tid] = this->build_monitor(tid);

#ifdef ENABLE_MPI
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new Monitor_reduction_mpi<B>(params.src->K,
	                                                 params.mnt->n_frame_errors,
	                                                 this->monitor,
	                                                 std::this_thread::get_id(),
	                                                 params.mpi_comm_freq,
	                                                 params.src->n_frames);
#else
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new Monitor_reduction<B>(params.src->K,
	                                             params.mnt->n_frame_errors,
	                                             this->monitor,
	                                             params.src->n_frames);
#endif
}

template <typename B, typename R, typename Q>
BFER<B,R,Q>
::~BFER()
{
	release_objects();

	if (monitor_red != nullptr) { delete monitor_red; monitor_red = nullptr; }
	if (dumper_red  != nullptr) { delete dumper_red;  dumper_red  = nullptr; }

	for (auto tid = 0; tid < params.n_threads; tid++)
	{
		if (monitor[tid] != nullptr) { delete monitor[tid]; monitor[tid] = nullptr; }
		if (dumper [tid] != nullptr) { delete dumper [tid]; dumper [tid] = nullptr; }
	}

	if (terminal != nullptr) { delete terminal; terminal = nullptr; }
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::build_communication_chain(const int tid)
{
	try
	{
		this->_build_communication_chain(tid);

		for (auto& duration : this->durations[tid])
			duration.second = std::chrono::nanoseconds(0);

		if (params.err_track_enable)
			this->monitor[tid]->add_handler_fe(std::bind(&Dumper::add, this->dumper[tid], std::placeholders::_1));
	}
	catch (std::exception const& e)
	{
		Monitor<B>::stop();

		mutex_exception.lock();
		if (prev_err_message != e.what())
		{
			std::cerr << apply_on_each_line(e.what(), &format_error) << std::endl;
			prev_err_message = e.what();
		}
		mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::_build_communication_chain(const int tid)
{
	// by default, do nothing
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::launch()
{
	this->terminal = this->build_terminal();

	codec.launch_precompute();

	// for each SNR to be simulated
	for (snr = params.snr_min; snr <= params.snr_max; snr += params.snr_step)
	{
		if (params.snr_type == "EB")
		{
			snr_b = snr;
			snr_s = ebn0_to_esn0(snr_b, bit_rate, params.mdm->bps);
		}
		else //if(params.sim->snr_type == "ES")
		{
			snr_s = snr;
			snr_b = esn0_to_ebn0(snr_s, bit_rate, params.mdm->bps);
		}
		sigma = esn0_to_sigma(snr_s, params.mdm->upf);

		this->terminal->set_esn0(snr_s);
		this->terminal->set_ebn0(snr_b);

		if (params.err_track_revert)
		{
			// dirty hack to override simulation params
			auto *params_writable = const_cast<factory::BFER::parameters*>(&params);

			if (this->params.src->type != "AZCW")
				params_writable->src->path = params.err_track_path + "_" + std::to_string(snr_b) + ".src";

			if (this->params.coset)
				params_writable->enc->path = params.err_track_path + "_" + std::to_string(snr_b) + ".enc";

			params_writable->chn->path = params.err_track_path + "_" + std::to_string(snr_b) + ".chn";

			std::ifstream file(params_writable->chn->path, std::ios::binary);
			if (file.is_open())
			{
				file.read((char*)&max_fra, sizeof(max_fra));
				file.close();
			}
			else
			{
				std::stringstream message;
				message << "Impossible to read the 'chn' file ('chn' = " << params_writable->chn->path << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}

		codec.snr_precompute(this->sigma);

		// build the communication chain in multi-threaded mode
		std::vector<std::thread> threads(params.n_threads -1);
		for (auto tid = 1; tid < params.n_threads; tid++)
			threads[tid -1] = std::thread(BFER<B,R,Q>::start_thread_build_comm_chain, this, tid);

		BFER<B,R,Q>::start_thread_build_comm_chain(this, 0);

		// join the slave threads with the master thread
		for (auto tid = 1; tid < params.n_threads; tid++)
			threads[tid -1].join();

		if (!Monitor<B>::is_over())
		{
			if (!params.ter->disabled && snr == params.snr_min && !params.debug && !params.benchs
#ifdef ENABLE_MPI
			    && params.mpi_rank == 0
#endif
			    )
				terminal->legend(std::cout);

			// start the terminal to display BER/FER results
			std::thread term_thread;
			if (!params.ter->disabled && params.ter->frequency != std::chrono::nanoseconds(0) &&
			    !params.benchs   && !params.debug)
				// launch a thread dedicated to the terminal display
				term_thread = std::thread(BFER<B,R,Q>::start_thread_terminal, this);

			try
			{
				this->_launch();
			}
			catch (std::exception const& e)
			{
				Monitor<B>::stop();
				std::cerr << apply_on_each_line(e.what(), &format_error) << std::endl;
			}

			// stop the terminal
			if (!params.ter->disabled && params.ter->frequency != std::chrono::nanoseconds(0) &&
			    !params.benchs && !params.debug)
			{
				stop_terminal = true;
				cond_terminal.notify_all();
				// wait the terminal thread to finish
				term_thread.join();
				stop_terminal = false;
			}

			if (!params.ter->disabled && !params.benchs && terminal != nullptr
#ifdef ENABLE_MPI
				 && params.mpi_rank == 0
#endif
			   )
			{
				if (params.debug && !Monitor<B>::is_over())
					terminal->legend(std::cout);

				time_reduction(true);

				if (!Monitor<B>::is_over())
					terminal->final_report(std::cout);
			}

			if (this->dumper_red != nullptr)
			{
				this->dumper_red->dump(params.err_track_path + "_" + std::to_string(snr_b));
				this->dumper_red->clear();
			}

			this->monitor_red->reset();
		}

		this->release_objects();

		if (Monitor<B>::is_over())
			break;
	}

	if (params.time_report && !params.benchs
#ifdef ENABLE_MPI
	    && params.mpi_rank == 0
#endif
	   )
		time_report();
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::release_objects()
{
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::time_reduction(const bool is_snr_done)
{
	for (auto& duration : durations_red)
		duration.second = std::chrono::nanoseconds(0);

	for (auto tid = 0; tid < params.n_threads; tid++)
		for (auto& duration : durations[tid])
			durations_red[duration.first] += duration.second;

	if (is_snr_done)
		for (auto tid = 0; tid < params.n_threads; tid++)
			for (auto& duration : durations[tid])
				durations_sum[duration.first] += duration.second;
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::time_report(std::ostream &stream)
{
	if (durations_sum.size() >= 1)
	{
		auto d_total = std::chrono::nanoseconds(0);
		for (auto& duration : durations_sum)
			if (!duration.first.second.empty())
				d_total += duration.first.second[0] != '-' ? duration.second : std::chrono::nanoseconds(0);
		auto total_sec = ((float)d_total.count()) * 0.000000001f;

		auto max_chars = 0;
		for (auto& duration : durations_sum)
			max_chars = std::max(max_chars, (int)duration.first.second.length());

		stream << "#" << std::endl;
		stream << "# " << format("Time report", Style::BOLD | Style::UNDERLINED) << " (the time of the threads is cumulated)" << std::endl;

		auto prev_sec = 0.f;
		for (auto& duration : durations_sum)
		{
			if (duration.second.count() != 0 && !duration.first.second.empty())
			{
				std::string key = "";
				const auto cur_sec = ((float)duration.second.count()) * 0.000000001f;
				auto cur_pc  = 0.f;
				if (duration.first.second[0] != '-')
				{
					cur_pc  = (cur_sec / total_sec) * 100.f;
					key = format("* " + duration.first.second, Style::BOLD);
					prev_sec = cur_sec;
				}
				else
				{
					cur_pc  = (prev_sec != 0.f) ? (cur_sec / prev_sec) * 100.f : 0.f;
					key = format("  " + duration.first.second, Style::BOLD | Style::ITALIC);
				}

				const auto n_spaces = max_chars - (int)duration.first.second.length();
				std::string str_spaces = "";
				for (auto i = 0; i < n_spaces; i++) str_spaces += " ";

				stream << "# " << key << str_spaces << ": "
				       << std::setw(9) << std::fixed << std::setprecision(3) << cur_sec << " sec ("
				       << std::setw(5) << std::fixed << std::setprecision(2) << cur_pc  << "%)";

				if (data_sizes.find(duration.first) != data_sizes.end())
				{
					const auto n_bits_per_fra = data_sizes[duration.first] / params.src->n_frames;
					const auto n_fra = this->monitor_red->get_n_analyzed_fra_historic();
					const auto mbps = ((float)(n_bits_per_fra * n_fra) / cur_sec) * 0.000001f;
					const auto inter_lvl = (float)params.src->n_frames;
					const auto lat = (((float)duration.second.count() * 0.001f) / (float)n_fra) * inter_lvl;

					stream << " - " << std::setw(9) << mbps << " Mb/s"
					       << " - " << std::setw(9) << lat  << " us";
				}

				stream << std::endl;
			}
		}

		stream << "#   ----------------------------------" << std::endl;
		const std::string total_str = "TOTAL";
		const auto n_spaces = max_chars - (int)total_str.length();
		std::string str_spaces = "";
		for (auto i = 0; i < n_spaces; i++) str_spaces += " ";
		stream << "# " << format("* " + total_str, Style::BOLD) << str_spaces << ": "
		       << std::setw(9) << std::fixed << std::setprecision(3) << total_sec << " sec" << std::endl;
		stream << "#" << std::endl;
	}
}

template <typename B, typename R, typename Q>
Monitor<B>* BFER<B,R,Q>
::build_monitor(const int tid)
{
	return factory::Monitor::build<B>(*params.mnt);
}

template <typename B, typename R, typename Q>
Terminal_BFER<B>* BFER<B,R,Q>
::build_terminal()
{
	return factory::Terminal_BFER::build<B>(*params.ter, *this->monitor_red);
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::start_thread_terminal(BFER<B,R,Q> *simu)
{
	if (simu->terminal != nullptr && simu->monitor_red != nullptr)
	{
		const auto sleep_time = simu->params.ter->frequency - std::chrono::milliseconds(0);

		while (!simu->stop_terminal)
		{
			std::unique_lock<std::mutex> lock(simu->mutex_terminal);
			if (simu->cond_terminal.wait_for(lock, sleep_time) == std::cv_status::timeout)
			{
				simu->time_reduction();
				simu->terminal->temp_report(std::clog); // display statistics in the terminal
			}
		}
	}
	else
		std::clog << format_warning("Terminal is not allocated: the temporal report can't be called.") << std::endl;
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid)
{
	simu->build_communication_chain(tid);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::BFER<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
