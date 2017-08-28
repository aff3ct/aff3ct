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
#include "Module/Monitor/BFER/Monitor_BFER_reduction_mpi.hpp"
#endif

#include "../../Factory/Module/Monitor/Monitor.hpp"
#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R, typename Q>
BFER<C,B,R,Q>
::BFER(const factory::BFER::parameters<C>& params)
: Simulation(),
  params(params),

  stop_terminal(false),

  barrier(params.n_threads),

  bit_rate((float)params.src.K / (float)params.cdc.N),

  snr  (0.f),
  snr_s(0.f),
  snr_b(0.f),
  sigma(0.f),

  max_fra(0),

  monitor    (params.n_threads, nullptr),
  monitor_red(                  nullptr),
  dumper     (params.n_threads, nullptr),
  dumper_red (                  nullptr),
  terminal   (                  nullptr)
{
#ifdef ENABLE_MPI
	if (params.debug)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "The debug modes is unavailable in MPI.");
#endif

	if (params.n_threads < 1)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << params.n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (params.err_track_enable)
	{
		for (auto tid = 0; tid < params.n_threads; tid++)
			dumper[tid] = new tools::Dumper(params.src.n_frames);

		std::vector<tools::Dumper*> dumpers;
		for (auto tid = 0; tid < params.n_threads; tid++)
			dumpers.push_back(dumper[tid]);

		dumper_red = new tools::Dumper_reduction(dumpers, params.src.n_frames);
	}

	modules["monitor"] = std::vector<module::Module*>(params.n_threads, nullptr);
	for (auto tid = 0; tid < params.n_threads; tid++)
	{
		this->monitor[tid] = this->build_monitor(tid);
		modules["monitor"][tid] = this->monitor[tid];
	}

#ifdef ENABLE_MPI
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new module::Monitor_BFER_reduction_mpi<B>(params.src->K,
	                                                              params.mnt->n_frame_errors,
	                                                              this->monitor,
	                                                              std::this_thread::get_id(),
	                                                              params.mpi_comm_freq,
	                                                              params.src->n_frames);
#else
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new module::Monitor_BFER_reduction<B>(params.src.K,
	                                                          params.mnt.n_frame_errors,
	                                                          this->monitor,
	                                                          params.src.n_frames);
#endif
}

template <class C, typename B, typename R, typename Q>
BFER<C,B,R,Q>
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

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::build_communication_chain(const int tid)
{
	try
	{
		this->_build_communication_chain(tid);

		if (params.err_track_enable)
			this->monitor[tid]->add_handler_fe(std::bind(&tools::Dumper::add, this->dumper[tid], std::placeholders::_1));
	}
	catch (std::exception const& e)
	{
		module::Monitor::stop();

		mutex_exception.lock();
		if (prev_err_message != e.what())
		{
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
			prev_err_message = e.what();
		}
		mutex_exception.unlock();
	}
}

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::_build_communication_chain(const int tid)
{
	// by default, do nothing
}

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::launch()
{
	this->terminal = this->build_terminal();

	if (!this->params.err_track_revert)
	{
		// build the communication chain in multi-threaded mode
		std::vector<std::thread> threads(params.n_threads -1);
		for (auto tid = 1; tid < params.n_threads; tid++)
			threads[tid -1] = std::thread(BFER<C,B,R,Q>::start_thread_build_comm_chain, this, tid);

		BFER<C,B,R,Q>::start_thread_build_comm_chain(this, 0);

		// join the slave threads with the master thread
		for (auto tid = 1; tid < params.n_threads; tid++)
			threads[tid -1].join();

		if (module::Monitor::is_over())
		{
			this->release_objects();
			return;
		}
	}

	// for each SNR to be simulated
	for (snr = params.snr_min; snr <= params.snr_max; snr += params.snr_step)
	{
		if (params.snr_type == "EB")
		{
			snr_b = snr;
			snr_s = tools::ebn0_to_esn0(snr_b, bit_rate, params.mdm.bps);
		}
		else // if (params.sim->snr_type == "ES")
		{
			snr_s = snr;
			snr_b = tools::esn0_to_ebn0(snr_s, bit_rate, params.mdm.bps);
		}
		sigma = tools::esn0_to_sigma(snr_s, params.mdm.upf);

		this->terminal->set_esn0(snr_s);
		this->terminal->set_ebn0(snr_b);

		if (this->params.err_track_revert)
		{
			this->release_objects();
			this->monitor_red->clear_callbacks();

			// dirty hack to override simulation params
			auto &params_writable = const_cast<factory::BFER::parameters<C>&>(params);

			if (this->params.src.type != "AZCW")
				params_writable.src.path = params.err_track_path + "_" + std::to_string(snr_b) + ".src";

			if (this->params.coset)
				params_writable.cdc.enc.path = params.err_track_path + "_" + std::to_string(snr_b) + ".enc";

			params_writable.chn.path = params.err_track_path + "_" + std::to_string(snr_b) + ".chn";

			std::ifstream file(this->params.chn.path, std::ios::binary);
			if (file.is_open())
			{
				file.read((char*)&max_fra, sizeof(max_fra));
				file.close();
			}
			else
			{
				std::stringstream message;
				message << "Impossible to read the 'chn' file ('chn' = " << this->params.chn.path << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			// build the communication chain in multi-threaded mode
			std::vector<std::thread> threads(params.n_threads -1);
			for (auto tid = 1; tid < params.n_threads; tid++)
				threads[tid -1] = std::thread(BFER<C,B,R,Q>::start_thread_build_comm_chain, this, tid);

			BFER<C,B,R,Q>::start_thread_build_comm_chain(this, 0);

			// join the slave threads with the master thread
			for (auto tid = 1; tid < params.n_threads; tid++)
				threads[tid -1].join();

			if (module::Monitor::is_over())
			{
				this->release_objects();
				return;
			}
		}

		// enable the debug mode in the modules
		if (params.debug)
			for (auto &m : modules)
				for (auto mm : m.second)
					if (mm != nullptr)
						for (auto &p : mm->processes)
						{
							p.second->set_debug(true);
							if (params.debug_limit)
								p.second->set_debug_limit((uint32_t)params.debug_limit);
							if (params.debug_precision)
								p.second->set_debug_precision((uint8_t)params.debug_precision);
						}

#ifdef ENABLE_MPI
		if (((!params.ter->disabled && snr == params.snr_min && !params.debug) ||
		    (params.statistics && !params.debug)) && params.mpi_rank == 0)
#else
		if (((!params.ter.disabled && snr == params.snr_min && !params.debug) ||
		    (params.statistics && !params.debug)))
#endif
			terminal->legend(std::cout);

		// start the terminal to display BER/FER results
		std::thread term_thread;
		if (!params.ter.disabled && params.ter.frequency != std::chrono::nanoseconds(0) && !params.debug)
			// launch a thread dedicated to the terminal display
			term_thread = std::thread(BFER<C,B,R,Q>::start_thread_terminal, this);

		auto simu_error = false;
		try
		{
			this->_launch();
		}
		catch (std::exception const& e)
		{
			module::Monitor::stop();
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
			simu_error = true;
		}

		// stop the terminal
		if (!params.ter.disabled && params.ter.frequency != std::chrono::nanoseconds(0) && !params.debug)
		{
			stop_terminal = true;
			cond_terminal.notify_all();
			// wait the terminal thread to finish
			term_thread.join();
			stop_terminal = false;
		}

#ifdef ENABLE_MPI
		if (!params.ter.disabled && terminal != nullptr && !simu_error && params.mpi_rank == 0)
#else
		if (!params.ter.disabled && terminal != nullptr && !simu_error)
#endif
		{
			if (params.debug)
				terminal->legend(std::cout);

			terminal->final_report(std::cout);

			if (params.statistics)
				display_stats();
		}

		if (this->dumper_red != nullptr && !simu_error)
		{
			this->dumper_red->dump(params.err_track_path + "_" + std::to_string(snr_b));
			this->dumper_red->clear();
		}

		this->monitor_red->reset();
		for (auto &m : modules)
			for (auto mm : m.second)
				if (mm != nullptr)
					for (auto &p : mm->processes)
						p.second->reset_stats();

		if (module::Monitor::is_over())
			break;
	}

	this->release_objects();
}

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::release_objects()
{
}

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::display_stats(std::ostream &stream)
{
	size_t max_chars = 0;
	auto d_total = std::chrono::nanoseconds(0);
	for (auto &m : modules)
		for (auto i = 0; i < (int)m.second.size(); i++)
			if (m.second[i] != nullptr)
				for (auto &p : m.second[i]->processes)
				{
					d_total += p.second->get_duration_total();
					std::max(max_chars, m.second[i]->get_short_name().size() + p.second->get_name().size());
				}
	auto total_sec = ((float)d_total.count()) * 0.000000001f;

	if (d_total.count())
	{
		stream << "#" << std::endl;
		stream << tools::format("# -------------------------------------------||------------------------------||--------------------------------||--------------------------------", tools::Style::BOLD) << std::endl;
		stream << tools::format("#      Statistics for the given process      ||       Basic statistics       ||       Measured throughput      ||        Measured latency        ", tools::Style::BOLD) << std::endl;
		stream << tools::format("#        ('*' = all the sub-processes)       ||        on the process        ||     considering the output     ||     considering the output     ", tools::Style::BOLD) << std::endl;
		stream << tools::format("# -------------------------------------------||------------------------------||--------------------------------||--------------------------------", tools::Style::BOLD) << std::endl;
		stream << tools::format("# -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------", tools::Style::BOLD) << std::endl;
		stream << tools::format("#       MODULE |           PROCESS |     SUB ||    CALLS |     TIME |   PERC ||  AVERAGE |  MINIMUM |  MAXIMUM ||  AVERAGE |  MINIMUM |  MAXIMUM ", tools::Style::BOLD) << std::endl;
		stream << tools::format("#              |                   | PROCESS ||          |      (s) |    (%) ||   (Mb/s) |   (Mb/s) |   (Mb/s) ||     (us) |     (us) |     (us) ", tools::Style::BOLD) << std::endl;
		stream << tools::format("# -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------", tools::Style::BOLD) << std::endl;

#ifdef _WIN32
		auto P = 1;
#else
		auto P = 2;
#endif

		for (auto &m : modules)
		{
			if (m.second[0] != nullptr)
			{
				for (auto &p : m.second[0]->processes)
				{
					size_t n_elmts = p.second->out.size() ? p.second->out[0].get_n_elmts() :
					                                        p.second->in [0].get_n_elmts();

					auto module = m.second[0]->get_short_name();
					auto process = p.second->get_name();
					uint32_t n_calls = 0;
					auto tot_duration = std::chrono::nanoseconds(0);
					auto min_duration = m.second[0]->processes[p.first]->get_duration_max();
					auto max_duration = std::chrono::nanoseconds(0);
					for (auto &mm : m.second)
					{
						n_calls += mm->processes[p.first]->get_n_calls();
						tot_duration += mm->processes[p.first]->get_duration_total();
						min_duration = std::min(min_duration, mm->processes[p.first]->get_duration_min());
						max_duration = std::max(max_duration, mm->processes[p.first]->get_duration_max());
					}

					if (n_calls)
					{
						auto tot_dur = ((float)tot_duration.count()) * 0.000000001f;
						auto percent = (tot_dur / total_sec) * 100.f;
						auto avg_thr = (float)(n_calls * n_elmts) / ((float)tot_duration.count() * 0.001f);
						auto min_thr = (float)(1.f     * n_elmts) / ((float)max_duration.count() * 0.001f);
						auto max_thr = (float)(1.f     * n_elmts) / ((float)min_duration.count() * 0.001f);
						auto avg_lat = (float)(tot_duration.count() * 0.001f) / n_calls;
						auto min_lat = (float)(min_duration.count() * 0.001f);
						auto max_lat = (float)(max_duration.count() * 0.001f);

						unsigned l1 = 99999999;
						unsigned l2 = 99999.99;

						std::stringstream ssmodule, ssprocess, sssp, ssn_calls, sstot_dur, sspercent;
						std::stringstream ssavg_thr, ssmin_thr, ssmax_thr;
						std::stringstream ssavg_lat, ssmin_lat, ssmax_lat;

						ssmodule  << std::setprecision(                   2) <<                                   std::fixed  << std::setw(12) << module;
						ssprocess << std::setprecision(                   2) <<                                   std::fixed  << std::setw(17) << process;
						sssp      << std::setprecision(                   2) <<                                   std::fixed  << std::setw( 7) << "*";
						ssn_calls << std::setprecision(n_calls > l1 ? P : 2) << (n_calls > l1 ? std::scientific : std::fixed) << std::setw( 8) << n_calls;
						sstot_dur << std::setprecision(tot_dur > l1 ? P : 2) << (tot_dur > l1 ? std::scientific : std::fixed) << std::setw( 8) << tot_dur;
						sspercent << std::setprecision(                   2) <<                                   std::fixed  << std::setw( 6) << percent;
						ssavg_thr << std::setprecision(avg_thr > l1 ? P : 2) << (avg_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << avg_thr;
						ssmin_thr << std::setprecision(min_thr > l1 ? P : 2) << (min_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << min_thr;
						ssmax_thr << std::setprecision(max_thr > l1 ? P : 2) << (max_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << max_thr;
						ssavg_lat << std::setprecision(avg_lat > l1 ? P : 2) << (avg_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << avg_lat;
						ssmin_lat << std::setprecision(min_lat > l1 ? P : 2) << (min_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << min_lat;
						ssmax_lat << std::setprecision(max_lat > l1 ? P : 2) << (max_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << max_lat;

						std::string spercent = sspercent.str();
						     if (percent > 50.0f) spercent = tools::format(spercent, tools::FG::INTENSE | tools::FG::RED);
						else if (percent > 25.0f) spercent = tools::format(spercent, tools::FG::INTENSE | tools::FG::ORANGE);
						else if (percent > 12.5f) spercent = tools::format(spercent, tools::FG::INTENSE | tools::FG::GREEN);
						else if (percent <  5.0f) spercent = tools::format(spercent,                      tools::FG::GRAY);

						stream << "# ";
						stream << ssmodule .str() << tools::format(" | ",  tools::Style::BOLD)
						       << ssprocess.str() << tools::format(" | ",  tools::Style::BOLD)
						       << sssp     .str() << tools::format(" || ", tools::Style::BOLD)
						       << ssn_calls.str() << tools::format(" | ",  tools::Style::BOLD)
						       << sstot_dur.str() << tools::format(" | ",  tools::Style::BOLD)
						       << spercent        << tools::format(" || ", tools::Style::BOLD)
						       << ssavg_thr.str() << tools::format(" | ",  tools::Style::BOLD)
						       << ssmin_thr.str() << tools::format(" | ",  tools::Style::BOLD)
						       << ssmax_thr.str() << tools::format(" || ", tools::Style::BOLD)
						       << ssavg_lat.str() << tools::format(" | ",  tools::Style::BOLD)
						       << ssmin_lat.str() << tools::format(" | ",  tools::Style::BOLD)
						       << ssmax_lat.str() << ""
						       << std::endl;

						for (auto &sp : p.second->get_registered_duration())
						{
							uint32_t rn_calls = 0;
							auto rtot_duration = std::chrono::nanoseconds(0);
							auto rmin_duration = m.second[0]->processes[p.first]->get_registered_duration_max(sp);
							auto rmax_duration = std::chrono::nanoseconds(0);
							for (auto &mm : m.second)
							{
								rn_calls += mm->processes[p.first]->get_registered_n_calls(sp);
								rtot_duration += mm->processes[p.first]->get_registered_duration_total(sp);
								rmin_duration = std::min(rmin_duration, mm->processes[p.first]->get_registered_duration_min(sp));
								rmax_duration = std::max(rmax_duration, mm->processes[p.first]->get_registered_duration_max(sp));
							}

							if (rn_calls)
							{
								auto rn_elmts = (n_elmts * n_calls) / rn_calls;
								auto rtot_dur = ((float)rtot_duration.count()) * 0.000000001f;
								auto rpercent = (rtot_dur / tot_dur) * 100.f;
								auto ravg_thr = (float)(rn_calls * rn_elmts) / ((float)rtot_duration.count() * 0.001f);
								auto rmin_thr = (float)(1.f      * rn_elmts) / ((float)rmax_duration.count() * 0.001f);
								auto rmax_thr = (float)(1.f      * rn_elmts) / ((float)rmin_duration.count() * 0.001f);
								auto ravg_lat = (float)(rtot_duration.count() * 0.001f) / rn_calls;
								auto rmin_lat = (float)(rmin_duration.count() * 0.001f);
								auto rmax_lat = (float)(rmax_duration.count() * 0.001f);

								std::stringstream sssp, ssrn_calls, ssrtot_dur, ssrpercent;
								std::stringstream ssravg_thr, ssrmin_thr, ssrmax_thr;
								std::stringstream ssravg_lat, ssrmin_lat, ssrmax_lat;

								sssp       << std::setprecision(                    2) <<                                    std::fixed  << std::setw(7) << sp;
								ssrn_calls << std::setprecision(rn_calls > l1 ? P : 2) << (rn_calls > l1 ? std::scientific : std::fixed) << std::setw(8) << rn_calls;
								ssrtot_dur << std::setprecision(rtot_dur > l1 ? P : 2) << (rtot_dur > l1 ? std::scientific : std::fixed) << std::setw(8) << rtot_dur;
								ssrpercent << std::setprecision(                    2) <<                                    std::fixed  << std::setw(6) << rpercent;
								ssravg_thr << std::setprecision(ravg_thr > l1 ? P : 2) << (ravg_thr > l2 ? std::scientific : std::fixed) << std::setw(8) << ravg_thr;
								ssrmin_thr << std::setprecision(rmin_thr > l1 ? P : 2) << (rmin_thr > l2 ? std::scientific : std::fixed) << std::setw(8) << rmin_thr;
								ssrmax_thr << std::setprecision(rmax_thr > l1 ? P : 2) << (rmax_thr > l2 ? std::scientific : std::fixed) << std::setw(8) << rmax_thr;
								ssravg_lat << std::setprecision(ravg_lat > l1 ? P : 2) << (ravg_lat > l2 ? std::scientific : std::fixed) << std::setw(8) << ravg_lat;
								ssrmin_lat << std::setprecision(rmin_lat > l1 ? P : 2) << (rmin_lat > l2 ? std::scientific : std::fixed) << std::setw(8) << rmin_lat;
								ssrmax_lat << std::setprecision(rmax_lat > l1 ? P : 2) << (rmax_lat > l2 ? std::scientific : std::fixed) << std::setw(8) << rmax_lat;

								stream << "# ";
								stream << tools::format(ssmodule  .str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssprocess .str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(sssp      .str(), tools::Style::ITALIC) << tools::format(" || ", tools::Style::BOLD)
								       << tools::format(ssrn_calls.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssrtot_dur.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssrpercent.str(), tools::Style::ITALIC) << tools::format(" || ", tools::Style::BOLD)
								       << tools::format(ssravg_thr.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssrmin_thr.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssrmax_thr.str(), tools::Style::ITALIC) << tools::format(" || ", tools::Style::BOLD)
								       << tools::format(ssravg_lat.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssrmin_lat.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
								       << tools::format(ssrmax_lat.str(), tools::Style::ITALIC) << ""
								       << std::endl;
							}
						}
					}
				}
			}
		}
		stream << "#" << std::endl;
	}
}

template <class C, typename B, typename R, typename Q>
module::Monitor_BFER<B>* BFER<C,B,R,Q>
::build_monitor(const int tid)
{
	return factory::Monitor_BFER::build<B>(params.mnt);
}

template <class C, typename B, typename R, typename Q>
tools::Terminal_BFER<B>* BFER<C,B,R,Q>
::build_terminal()
{
	return factory::Terminal_BFER::build<B>(params.ter, *this->monitor_red);
}

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::start_thread_terminal(BFER<C,B,R,Q> *simu)
{
	if (simu->terminal != nullptr && simu->monitor_red != nullptr)
	{
		const auto sleep_time = simu->params.ter.frequency - std::chrono::milliseconds(0);

		while (!simu->stop_terminal)
		{
			std::unique_lock<std::mutex> lock(simu->mutex_terminal);
			if (simu->cond_terminal.wait_for(lock, sleep_time) == std::cv_status::timeout)
				simu->terminal->temp_report(std::clog); // display statistics in the terminal
		}
	}
	else
		std::clog << tools::format_warning("Terminal is not allocated: the temporal report can't be called.")
		          << std::endl;
}

template <class C, typename B, typename R, typename Q>
void BFER<C,B,R,Q>
::start_thread_build_comm_chain(BFER<C,B,R,Q> *simu, const int tid)
{
	simu->build_communication_chain(tid);
}
}
}
