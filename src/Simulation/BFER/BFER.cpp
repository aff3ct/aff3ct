#include <cmath>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Statistics/Statistics.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#ifdef ENABLE_MPI
#include "Module/Monitor/BFER/Monitor_BFER_reduction_mpi.hpp"
#endif

#include "Factory/Module/Monitor/Monitor.hpp"
#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER<B,R,Q>
::BFER(const factory::BFER::parameters& params)
: Simulation(params),
  params(params),

  barrier(params.n_threads),

  bit_rate((float)params.src->K / (float)params.cdc->N),

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
	if (params.n_threads < 1)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << params.n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (params.err_track_enable)
	{
		for (auto tid = 0; tid < params.n_threads; tid++)
			dumper[tid] = new tools::Dumper();

		std::vector<tools::Dumper*> dumpers;
		for (auto tid = 0; tid < params.n_threads; tid++)
			dumpers.push_back(dumper[tid]);

		dumper_red = new tools::Dumper_reduction(dumpers);
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
	this->monitor_red = new module::Monitor_BFER_reduction<B>(params.src->K,
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
::_build_communication_chain()
{
	// build the communication chain in multi-threaded mode
	std::vector<std::thread> threads(params.n_threads -1);
	for (auto tid = 1; tid < params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER<B,R,Q>::start_thread_build_comm_chain, this, tid);

	BFER<B,R,Q>::start_thread_build_comm_chain(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < params.n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::launch()
{
	this->terminal = this->build_terminal();

	if (!this->params.err_track_revert)
	{
		this->build_communication_chain();

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
			snr_s = tools::ebn0_to_esn0(snr_b, bit_rate, params.mdm->bps);
		}
		else // if (params.sim->snr_type == "ES")
		{
			snr_s = snr;
			snr_b = tools::esn0_to_ebn0(snr_s, bit_rate, params.mdm->bps);
		}
		sigma = tools::esn0_to_sigma(snr_s, params.mdm->upf);

		this->terminal->set_esn0(snr_s);
		this->terminal->set_ebn0(snr_b);

		if (this->params.err_track_revert)
		{
			this->release_objects();
			this->monitor_red->clear_callbacks();

			// dirty hack to override simulation params
			auto &params_writable = const_cast<factory::BFER::parameters&>(params);

			std::stringstream s_snr_b;
			s_snr_b << std::setprecision(2) << std::fixed << snr_b;

			params_writable.src     ->path = params.err_track_path + "_" + s_snr_b.str() + ".src";
			params_writable.cdc->enc->path = params.err_track_path + "_" + s_snr_b.str() + ".enc";
			params_writable.chn     ->path = params.err_track_path + "_" + s_snr_b.str() + ".chn";

			std::ifstream file(this->params.chn->path, std::ios::binary);
			if (file.is_open())
			{
				file.read((char*)&max_fra, sizeof(max_fra));
				file.close();
			}
			else
			{
				std::stringstream message;
				message << "Impossible to read the 'chn' file ('chn' = " << this->params.chn->path << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			this->build_communication_chain();

			if (module::Monitor::is_over())
			{
				this->release_objects();
				return;
			}
		}

#ifdef ENABLE_MPI
		if (((!params.ter->disabled && snr == params.snr_min && !params.debug) ||
		    (params.statistics && !params.debug)) && params.mpi_rank == 0)
#else
		if (((!params.ter->disabled && snr == params.snr_min && !params.debug) ||
		    (params.statistics && !params.debug)))
#endif
			terminal->legend(std::cout);

		// start the terminal to display BER/FER results
#ifdef ENABLE_MPI
		if (!params.ter->disabled && params.ter->frequency != std::chrono::nanoseconds(0) && !params.debug
		    && params.mpi_rank == 0)
#else
		if (!params.ter->disabled && params.ter->frequency != std::chrono::nanoseconds(0) && !params.debug)
#endif
			terminal->start_temp_report(params.ter->frequency);

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

#ifdef ENABLE_MPI
		if (!params.ter->disabled && terminal != nullptr && !simu_error && params.mpi_rank == 0)
#else
		if (!params.ter->disabled && terminal != nullptr && !simu_error)
#endif
		{
			if (params.debug)
				terminal->legend(std::cout);

			terminal->final_report(std::cout);

			if (params.statistics)
			{
				std::vector<std::vector<module::Module*>> mod_vec;
				for (auto &vm : modules)
					mod_vec.push_back(vm.second);

				std::cout << "#" << std::endl;
				tools::Stats::show(mod_vec, std::cout);
				std::cout << "#" << std::endl;
			}
		}

		if (this->dumper_red != nullptr && !simu_error)
		{
			std::stringstream s_snr_b;
			s_snr_b << std::setprecision(2) << std::fixed << snr_b;

			this->dumper_red->dump(params.err_track_path + "_" + s_snr_b.str());
			this->dumper_red->clear();
		}

		this->monitor_red->reset();
		for (auto &m : modules)
			for (auto mm : m.second)
				if (mm != nullptr)
					for (auto &t : mm->tasks)
						t.second->reset_stats();

		if (module::Monitor::is_over())
			break;
	}

	this->release_objects();
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::release_objects()
{
}

template <typename B, typename R, typename Q>
module::Monitor_BFER<B>* BFER<B,R,Q>
::build_monitor(const int tid)
{
	return factory::Monitor_BFER::build<B>(*params.mnt);
}

template <typename B, typename R, typename Q>
tools::Terminal_BFER<B>* BFER<B,R,Q>
::build_terminal()
{
	return factory::Terminal_BFER::build<B>(*params.ter, *this->monitor_red);
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->__build_communication_chain(tid);

		if (simu->params.err_track_enable)
			simu->monitor[tid]->add_handler_fe(std::bind(&tools::Dumper::add, simu->dumper[tid], std::placeholders::_1));
	}
	catch (std::exception const& e)
	{
		module::Monitor::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
		{
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
			simu->prev_err_message = e.what();
		}
		simu->mutex_exception.unlock();
	}
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

