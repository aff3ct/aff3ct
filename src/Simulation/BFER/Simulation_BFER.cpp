#include <cmath>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Factory_monitor.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#ifdef ENABLE_MPI
#include "Module/Monitor/Standard/Monitor_reduction_mpi.hpp"
#endif

#include "Simulation_BFER.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const parameters& params, Codec<B,Q> &codec)
: Simulation(),

  stop_terminal(false),

  codec(codec),

  params(params),

  barrier(params.simulation.n_threads),

  snr  (0.f),
  snr_s(0.f),
  snr_b(0.f),
  sigma(0.f),

  monitor    (params.simulation.n_threads, nullptr),
  monitor_red(                             nullptr),
  dumper     (params.simulation.n_threads, nullptr),
  dumper_red (                             nullptr),
  terminal   (                             nullptr),

  durations(params.simulation.n_threads)
{
	if (params.simulation.n_threads < 1)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << params.simulation.n_threads << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (params.monitor.err_track_enable)
	{
		for (auto tid = 0; tid < params.simulation.n_threads; tid++)
			dumper[tid] = new Dumper(params.simulation.inter_frame_level);

		std::vector<Dumper*> dumpers;
		for (auto tid = 0; tid < params.simulation.n_threads; tid++)
			dumpers.push_back(dumper[tid]);

		dumper_red = new Dumper_reduction(dumpers, params.simulation.inter_frame_level);
	}

	for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		this->monitor[tid] = this->build_monitor(tid);

#ifdef ENABLE_MPI
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new Monitor_reduction_mpi<B>(this->params.code.K_info,
	                                                 this->params.monitor.n_frame_errors,
	                                                 this->monitor,
	                                                 std::this_thread::get_id(),
	                                                 this->params.simulation.mpi_comm_freq,
	                                                 this->params.simulation.inter_frame_level);
#else
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new Monitor_reduction<B>(this->params.code.K_info,
	                                             this->params.monitor.n_frame_errors,
	                                             this->monitor,
	                                             this->params.simulation.inter_frame_level);
#endif
}

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::~Simulation_BFER()
{
	release_objects();

	if (monitor_red != nullptr) { delete monitor_red; monitor_red = nullptr; }
	if (dumper_red  != nullptr) { delete dumper_red;  dumper_red  = nullptr; }

	for (auto tid = 0; tid < params.simulation.n_threads; tid++)
	{
		if (monitor[tid] != nullptr) { delete monitor[tid]; monitor[tid] = nullptr; }
		if (dumper [tid] != nullptr) { delete dumper [tid]; dumper [tid] = nullptr; }
	}

	if (terminal != nullptr) { delete terminal; terminal = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain(const int tid)
{
	try
	{
		this->_build_communication_chain(tid);

		for (auto& duration : this->durations[tid])
			duration.second = std::chrono::nanoseconds(0);

		if (params.monitor.err_track_enable)
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
void Simulation_BFER<B,R,Q>
::_build_communication_chain(const int tid)
{
	// by default, do nothing
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::launch()
{
	this->terminal = this->build_terminal();

	codec.launch_precompute();

	// for each SNR to be simulated
	for (snr = params.simulation.snr_min; snr <= params.simulation.snr_max; snr += params.simulation.snr_step)
	{
		if (params.simulation.snr_type == "EB")
		{
			snr_b = snr;
			snr_s = ebn0_to_esn0(snr_b, params.code.R, params.modulator.bits_per_symbol);
		}
		else //if(params.simulation.snr_type == "ES")
		{
			snr_s = snr;
			snr_b = esn0_to_ebn0(snr_s, params.code.R, params.modulator.bits_per_symbol);
		}
		sigma = esn0_to_sigma(snr_s, params.modulator.upsample_factor);

		this->terminal->set_esn0(snr_s);
		this->terminal->set_ebn0(snr_b);

		// dirty hack to override simulation params
		if (this->params.monitor.err_track_revert)
		{
			parameters *params_writable = const_cast<parameters*>(&this->params);
			const auto base_path = this->params.monitor.err_track_path;
			params_writable->source     .path = base_path + "_" + std::to_string(snr_b) + ".src";
			params_writable->encoder    .path = base_path + "_" + std::to_string(snr_b) + ".enc";
			params_writable->channel    .path = base_path + "_" + std::to_string(snr_b) + ".chn";
			if (this->params.interleaver.uniform)
				params_writable->interleaver.path = base_path + "_" + std::to_string(snr_b) + ".itl";
		}

		codec.snr_precompute(this->sigma);

		// build the communication chain in multi-threaded mode
		std::vector<std::thread> threads(this->params.simulation.n_threads -1);
		for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
			threads[tid -1] = std::thread(Simulation_BFER<B,R,Q>::start_thread_build_comm_chain, this, tid);

		Simulation_BFER<B,R,Q>::start_thread_build_comm_chain(this, 0);

		// join the slave threads with the master thread
		for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
			threads[tid -1].join();

		if (!Monitor<B>::is_over())
		{
			if (params.simulation.mpi_rank == 0 && !params.terminal.disabled && snr == params.simulation.snr_min &&
			    !params.simulation.debug && !this->params.simulation.benchs)
				terminal->legend(std::cout);

			// start the terminal to display BER/FER results
			std::thread term_thread;
			if (!this->params.terminal.disabled && this->params.terminal.frequency != std::chrono::nanoseconds(0) &&
			    !this->params.simulation.benchs && !this->params.simulation.debug)
				// launch a thread dedicated to the terminal display
				term_thread = std::thread(Simulation_BFER<B,R,Q>::start_thread_terminal, this);

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
			if (!this->params.terminal.disabled && this->params.terminal.frequency != std::chrono::nanoseconds(0) &&
			    !this->params.simulation.benchs && !this->params.simulation.debug)
			{
				stop_terminal = true;
				cond_terminal.notify_all();
				// wait the terminal thread to finish
				term_thread.join();
				stop_terminal = false;
			}

			if (this->params.simulation.mpi_rank == 0 &&
			    !this->params.terminal.disabled       &&
			    !this->params.simulation.benchs       &&
			    terminal != nullptr)
			{
				if (this->params.simulation.debug && !Monitor<B>::is_over())
					terminal->legend(std::cout);

				time_reduction(true);

				if (!Monitor<B>::is_over())
					terminal->final_report(std::cout);
			}

			if (this->dumper_red != nullptr)
			{
				this->dumper_red->dump(this->params.monitor.err_track_path + "_" + std::to_string(snr_b));
				this->dumper_red->clear();
			}

			this->monitor_red->reset();
		}

		this->release_objects();

		if (Monitor<B>::is_over())
			break;
	}

	if (this->params.simulation.time_report &&
	    !this->params.simulation.benchs     &&
	    this->params.simulation.mpi_rank == 0)
		time_report();
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::release_objects()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::time_reduction(const bool is_snr_done)
{
	for (auto& duration : durations_red)
		duration.second = std::chrono::nanoseconds(0);

	for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
		for (auto& duration : durations[tid])
			durations_red[duration.first] += duration.second;

	if (is_snr_done)
		for (auto tid = 0; tid < this->params.simulation.n_threads; tid++)
			for (auto& duration : durations[tid])
				durations_sum[duration.first] += duration.second;
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
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
					const auto n_bits_per_fra = data_sizes[duration.first] / params.simulation.inter_frame_level;
					const auto n_fra = this->monitor_red->get_n_analyzed_fra_historic();
					const auto mbps = ((float)(n_bits_per_fra * n_fra) / cur_sec) * 0.000001f;
					const auto inter_lvl = (float)params.simulation.inter_frame_level;
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
Monitor<B>* Simulation_BFER<B,R,Q>
::build_monitor(const int tid)
{
	return Factory_monitor<B>::build("STD",
	                                 this->params.code.K_info,
	                                 this->params.monitor.n_frame_errors,
	                                 this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
Terminal_BFER<B>* Simulation_BFER<B,R,Q>
::build_terminal()
{
	return new Terminal_BFER<B>(this->params.code.K_info,
	                            this->params.code.N_code,
	                            *this->monitor_red);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::start_thread_terminal(Simulation_BFER<B,R,Q> *simu)
{
	if (simu->terminal != nullptr && simu->monitor_red != nullptr)
	{
		const auto sleep_time = simu->params.terminal.frequency - std::chrono::milliseconds(0);

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
void Simulation_BFER<B,R,Q>
::start_thread_build_comm_chain(Simulation_BFER<B,R,Q> *simu, const int tid)
{
	simu->build_communication_chain(tid);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
