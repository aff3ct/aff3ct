#include <cmath>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Tools/general_utils.h"
#include "Tools/system_functions.h"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
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
::BFER(const factory::BFER::parameters& params_BFER)
: Simulation(params_BFER),
  params_BFER(params_BFER),

  barrier(params_BFER.n_threads),

  bit_rate((float)params_BFER.src->K / (float)params_BFER.cdc->N),
  noise(nullptr),

  monitor    (params_BFER.n_threads, nullptr),
  monitor_red(                       nullptr),
  dumper     (params_BFER.n_threads, nullptr),
  dumper_red (                       nullptr),
  terminal   (                       nullptr)
{
	if (params_BFER.n_threads < 1)
	{
		std::stringstream message;
		message << "'n_threads' has to be greater than 0 ('n_threads' = " << params_BFER.n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (params_BFER.err_track_enable)
	{
		for (auto tid = 0; tid < params_BFER.n_threads; tid++)
			dumper[tid] = new tools::Dumper();

		std::vector<tools::Dumper*> dumpers;
		for (auto tid = 0; tid < params_BFER.n_threads; tid++)
			dumpers.push_back(dumper[tid]);

		dumper_red = new tools::Dumper_reduction(dumpers);
	}

	modules["monitor"] = std::vector<module::Module*>(params_BFER.n_threads, nullptr);
	for (auto tid = 0; tid < params_BFER.n_threads; tid++)
	{
		this->monitor[tid] = this->build_monitor(tid);
		modules["monitor"][tid] = this->monitor[tid];
	}

#ifdef ENABLE_MPI
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new module::Monitor_BFER_reduction_mpi<B>(this->monitor,
	                                                              std::this_thread::get_id(),
	                                                              params_BFER.mpi_comm_freq);
#else
	// build a monitor to compute BER/FER (reduce the other monitors)
	this->monitor_red = new module::Monitor_BFER_reduction<B>(this->monitor);
#endif
}

template <typename B, typename R, typename Q>
BFER<B,R,Q>
::~BFER()
{
	release_objects();

	if (monitor_red != nullptr) { delete monitor_red; monitor_red = nullptr; }
	if (dumper_red  != nullptr) { delete dumper_red;  dumper_red  = nullptr; }

	for (auto tid = 0; tid < params_BFER.n_threads; tid++)
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
	std::vector<std::thread> threads(params_BFER.n_threads -1);
	for (auto tid = 1; tid < params_BFER.n_threads; tid++)
		threads[tid -1] = std::thread(BFER<B,R,Q>::start_thread_build_comm_chain, this, tid);

	BFER<B,R,Q>::start_thread_build_comm_chain(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < params_BFER.n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::launch()
{
	this->terminal = this->build_terminal();

	if (!this->params_BFER.err_track_revert)
	{
		this->build_communication_chain();

		if (module::Monitor::is_over())
		{
			this->release_objects();
			return;
		}
	}

	// for each NOISE to be simulated
	for (unsigned noise_idx = 0; noise_idx < params_BFER.noise_range.size(); noise_idx ++)
	{
		auto n = params_BFER.noise_range[noise_idx];

		if (this->noise != nullptr) delete noise;

		if (params_BFER.noise_type == "EBN0" || params_BFER.noise_type == "ESN0")
		{
			float esn0, ebn0;
			if (params_BFER.noise_type == "EBN0")
			{
				ebn0 = n;
				esn0 = tools::ebn0_to_esn0(ebn0, bit_rate, params_BFER.mdm->bps);
			}
			else // if (params_BFER.sim->noise_type == "ESN0")
			{
				esn0 = n;
				ebn0 = tools::esn0_to_ebn0(esn0, bit_rate, params_BFER.mdm->bps);
			}

			auto sigma = tools::esn0_to_sigma(esn0, params_BFER.mdm->upf);

			this->noise = new tools::Sigma<R>(sigma, ebn0, esn0);
		}
		else if (params_BFER.noise_type == "ROP")
		{
			this->noise = new tools::Received_optical_power<R>(n);
		}
		else if (params_BFER.noise_type == "EP")
		{
		    n = params_BFER.noise_range[params_BFER.noise_range.size() - noise_idx -1];

		    this->noise = new tools::Erasure_probability<R>(n);
		}
		else
		{
			std::stringstream message;
			message << "Unknown noise type ('noise_type' = " << params_BFER.noise_type << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->terminal->set_noise(*this->noise);


		if (this->params_BFER.err_track_revert)
		{
			this->release_objects();
			this->monitor_red->clear_callbacks();

			// dirty hack to override simulation params_BFER
			auto &params_BFER_writable = const_cast<factory::BFER::parameters&>(params_BFER);

			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_noise();

			params_BFER_writable.src     ->path = params_BFER.err_track_path + "_" + s_noise.str() + ".src";
			params_BFER_writable.cdc->enc->path = params_BFER.err_track_path + "_" + s_noise.str() + ".enc";
			params_BFER_writable.chn     ->path = params_BFER.err_track_path + "_" + s_noise.str() + ".chn";

			std::ifstream file(params_BFER.chn->path, std::ios::binary);
			if (file.is_open())
			{
				unsigned max_fra;
				file.read((char*)&max_fra, sizeof(max_fra));
				file.close();

				*const_cast<unsigned*>(&params_BFER.max_frame) = max_fra;
			}
			else
			{
				std::stringstream message;
				message << "Impossible to read the 'chn' file ('chn' = " << params_BFER.chn->path << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			this->build_communication_chain();

			if (module::Monitor::is_over())
			{
				this->release_objects();
				return;
			}
		}

		if (params_BFER.display_legend)
#ifdef ENABLE_MPI
			if (((!params_BFER.ter->disabled && noise_idx == 0 && !params_BFER.debug) ||
		 	   (params_BFER.statistics && !params_BFER.debug)) && params_BFER.mpi_rank == 0)
#else
			if (((!params_BFER.ter->disabled && noise_idx == 0 && !params_BFER.debug) ||
			    (params_BFER.statistics && !params_BFER.debug)))
#endif
				terminal->legend(std::cout);

		// start the terminal to display BER/FER results
#ifdef ENABLE_MPI
		if (!params_BFER.ter->disabled && params_BFER.ter->frequency != std::chrono::nanoseconds(0) && !params_BFER.debug
		    && params_BFER.mpi_rank == 0)
#else
		if (!params_BFER.ter->disabled && params_BFER.ter->frequency != std::chrono::nanoseconds(0) && !params_BFER.debug)
#endif
			terminal->start_temp_report(params_BFER.ter->frequency);

		try
		{
			this->_launch();
		}
		catch (std::exception const& e)
		{
			module::Monitor::stop();
			terminal->final_report(std::cout); // display final report to not lost last line overwritten by the error messages

			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
			this->simu_error = true;
		}

#ifdef ENABLE_MPI
		if (!params_BFER.ter->disabled && terminal != nullptr && !this->simu_error && params_BFER.mpi_rank == 0)
#else
		if (!params_BFER.ter->disabled && terminal != nullptr && !this->simu_error)
#endif
		{
			if (params_BFER.debug)
				terminal->legend(std::cout);

			terminal->final_report(std::cout);

			if (params_BFER.statistics)
			{
				std::vector<std::vector<const module::Module*>> mod_vec;
				for (auto &vm : modules)
				{
					std::vector<const module::Module*> sub_mod_vec;
					for (auto *m : vm.second)
						sub_mod_vec.push_back(m);
					mod_vec.push_back(sub_mod_vec);
				}

				std::cout << "#" << std::endl;
				tools::Stats::show(mod_vec, true, std::cout);
				std::cout << "#" << std::endl;
			}
		}

		if (this->dumper_red != nullptr && !this->simu_error)
		{
			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_noise();

			this->dumper_red->dump(params_BFER.err_track_path + "_" + s_noise.str());
			this->dumper_red->clear();
		}

		if (!params_BFER.crit_nostop && !params_BFER.err_track_revert && !module::Monitor::is_interrupt() &&
		    this->monitor_red->get_n_fe() < this->monitor_red->get_fe_limit() &&
		    (params_BFER.max_frame == 0 || this->monitor_red->get_n_fe() < params_BFER.max_frame))
			module::Monitor::stop();

		this->monitor_red->reset();
		for (auto &m : modules)
			for (auto mm : m.second)
				if (mm != nullptr)
					for (auto &t : mm->tasks)
						t->reset_stats();

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
	return factory::Monitor_BFER::build<B>(*params_BFER.mnt);
}

template <typename B, typename R, typename Q>
tools::Terminal_BFER<B>* BFER<B,R,Q>
::build_terminal()
{
	return factory::Terminal_BFER::build<B>(*params_BFER.ter, *this->monitor_red);
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->__build_communication_chain(tid);

		if (simu->params_BFER.err_track_enable)
			simu->monitor[tid]->add_handler_fe(std::bind(&tools::Dumper::add, simu->dumper[tid], std::placeholders::_1, std::placeholders::_2));
	}
	catch (std::exception const& e)
	{
		module::Monitor::stop();
		simu->simu_error = true;

		simu->mutex_exception.lock();

		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		std::string msg = e.what(); // get only the function signature
		tools::exception::no_backtrace = save;

		if (std::find(simu->prev_err_messages.begin(), simu->prev_err_messages.end(), msg) == simu->prev_err_messages.end())
		{
			// with backtrace if debug mode
			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);

			simu->prev_err_messages.push_back(msg); // save only the function signature
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

