#include <cmath>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Tools/general_utils.h"
#include "Tools/system_functions.h"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Display/Statistics/Statistics.hpp"
#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Monitor/Monitor.hpp"
#include "Factory/Tools/Display/Terminal/Terminal.hpp"

#include "Simulation/BFER/BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER<B,R,Q>
::BFER(const factory::BFER::parameters& params_BFER)
: Simulation(params_BFER),
  params_BFER(params_BFER),

  bit_rate((float)params_BFER.src->K / (float)params_BFER.cdc->N),

  monitor_mi(params_BFER.n_threads),
  monitor_er(params_BFER.n_threads),
  dumper    (params_BFER.n_threads)
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
			dumper[tid].reset(new tools::Dumper());

		dumper_red.reset(new tools::Dumper_reduction(dumper));
	}

	if (!params_BFER.noise->pdf_path.empty())
		distributions.reset(new tools::Distributions<R>(params_BFER.noise->pdf_path,
		                                                tools::Distribution_mode::SUMMATION,
		                                                params_BFER.mdm->rop_est_bits > 0));

	this->build_monitors ();
	this->build_reporters();

	this->terminal = this->build_terminal();
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
	if (!params_BFER.err_track_revert)
	{
		this->build_communication_chain();

		if (tools::Terminal::is_over())
			return;
	}

	int noise_begin = 0;
	int noise_end   = (int)params_BFER.noise->range.size();
	int noise_step  = 1;
	if (params_BFER.noise->type == "EP")
	{
		noise_begin = (int)params_BFER.noise->range.size()-1;
		noise_end   = -1;
		noise_step  = -1;
	}

	// for each NOISE to be simulated
	for (auto noise_idx = noise_begin; noise_idx != noise_end; noise_idx += noise_step)
	{
		this->noise.reset(params_BFER.noise->template build<R>(params_BFER.noise->range[noise_idx], bit_rate,
		                                                       params_BFER.mdm->bps, params_BFER.mdm->cpm_upf));

		// manage noise distributions to be sure it exists
		if (this->distributions != nullptr)
			this->distributions->read_distribution(this->noise->get_noise());

		if (params_BFER.err_track_revert)
		{
			this->monitor_er_red->clear_callbacks();

			if (this->monitor_mi_red != nullptr)
				this->monitor_mi_red->clear_callbacks();

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

			if (tools::Terminal::is_over())
				return;
		}

#ifdef AFF3CT_MPI
		if (params_BFER.mpi_rank == 0)
#endif
		if (params_BFER.display_legend)
			if ((!params_BFER.ter->disabled && noise_idx == noise_begin && !params_BFER.debug)
				|| (params_BFER.statistics && !params_BFER.debug))
				terminal->legend(std::cout);

#ifdef AFF3CT_MPI
		if (params_BFER.mpi_rank == 0)
#endif
		// start the terminal to display BER/FER results
		if (!params_BFER.ter->disabled && params_BFER.ter->frequency != std::chrono::nanoseconds(0) &&
		    !params_BFER.debug)
			terminal->start_temp_report(params_BFER.ter->frequency);

		this->t_start_noise_point = std::chrono::steady_clock::now();

		try
		{
			this->_launch();
			module::Monitor_reduction::is_done_all(true, true); // final reduction
		}
		catch (std::exception const& e)
		{
			module::Monitor_reduction::is_done_all(true, true); // final reduction

			terminal->final_report(std::cout); // display final report to not lost last line overwritten by the error
			                                   // messages
			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
			this->simu_error = true;

			tools::Terminal::stop();
		}

#ifdef AFF3CT_MPI
		if (params_BFER.mpi_rank == 0)
#endif
		if (!params_BFER.ter->disabled && terminal != nullptr && !this->simu_error)
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
					for (auto& m : vm.second)
						sub_mod_vec.push_back(m);
					mod_vec.push_back(std::move(sub_mod_vec));
				}

				std::cout << "#" << std::endl;
				tools::Stats::show(mod_vec, true, std::cout);
				std::cout << "#" << std::endl;
			}
		}

		if (!params_BFER.crit_nostop && !params_BFER.err_track_revert && !tools::Terminal::is_interrupt() &&
		    !this->monitor_er_red->fe_limit_achieved() &&
		    (this->monitor_er_red->frame_limit_achieved() || this->stop_time_reached()))
			tools::Terminal::stop();


		if (params_BFER.mnt_er->err_hist != -1)
		{
			auto err_hist = monitor_er_red->get_err_hist();

			if (err_hist.get_n_values() != 0)
			{
				std::string noise_value;
				switch (this->noise->get_type())
				{
					case tools::Noise_type::SIGMA:
						if (params_BFER.noise->type == "EBN0")
							noise_value = std::to_string(dynamic_cast<tools::Sigma<>*>(this->noise.get())->get_ebn0());
						else //(params_BFER.noise_type == "ESN0")
							noise_value = std::to_string(dynamic_cast<tools::Sigma<>*>(this->noise.get())->get_esn0());
						break;
					case tools::Noise_type::ROP:
					case tools::Noise_type::EP:
						noise_value = std::to_string(this->noise->get_noise());
						break;
				}

				std::ofstream file_err_hist(params_BFER.mnt_er->err_hist_path + "_" + noise_value + ".txt");
				file_err_hist << "\"Number of error bits per wrong frame\"; \"Histogram (noise: " << noise_value
				              << this->noise->get_unity() << ", on " << err_hist.get_n_values() << " frames)\""
				              << std::endl;

				int max;
				if (params_BFER.mnt_er->err_hist == 0)
					max = err_hist.get_hist_max();
				else
					max = params_BFER.mnt_er->err_hist;
				err_hist.dump(file_err_hist, 0, max);
			}
		}

		if (this->dumper_red != nullptr && !this->simu_error)
		{
			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_noise();

			this->dumper_red->dump(params_BFER.err_track_path + "_" + s_noise.str());
			this->dumper_red->clear();
		}

		if (tools::Terminal::is_over())
			break;

		for (auto &m : modules)
			for (auto& mm : m.second)
				if (mm != nullptr)
					for (auto &t : mm->tasks)
						t->reset_stats();

		module::Monitor_reduction::reset_all();
		tools::Terminal::reset();
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<typename BFER<B,R,Q>::Monitor_MI_type> BFER<B,R,Q>
::build_monitor_mi(const int tid)
{
	return std::unique_ptr<typename BFER<B,R,Q>::Monitor_MI_type>(params_BFER.mnt_mi->build<B,R>());
}

template <typename B, typename R, typename Q>
std::unique_ptr<typename BFER<B,R,Q>::Monitor_BFER_type> BFER<B,R,Q>
::build_monitor_er(const int tid)
{
	bool count_unknown_values = params_BFER.noise->type == "EP";

	auto mnt_tmp = params_BFER.mnt_er->build<B>(count_unknown_values);
	auto mnt = std::unique_ptr<typename BFER<B,R,Q>::Monitor_BFER_type>(mnt_tmp);
	mnt->activate_err_histogram(params_BFER.mnt_er->err_hist != -1);

	return mnt;
}

template <typename B, typename R, typename Q>
std::unique_ptr<tools::Terminal> BFER<B,R,Q>
::build_terminal()
{
	return std::unique_ptr<tools::Terminal>(params_BFER.ter->build(this->reporters));
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::build_reporters()
{
	this->noise.reset(params_BFER.noise->template build<R>(0));

	auto reporter_noise = new tools::Reporter_noise<R>(this->noise, this->params_BFER.ter_sigma);
	this->reporters.push_back(std::unique_ptr<tools::Reporter_noise<R>>(reporter_noise));

	if (params_BFER.mnt_mutinfo)
	{
		auto reporter_MI = new tools::Reporter_MI<B,R>(*this->monitor_mi_red);
		this->reporters.push_back(std::unique_ptr<tools::Reporter_MI<B,R>>(reporter_MI));
	}

	auto reporter_BFER = new tools::Reporter_BFER<B>(*this->monitor_er_red);
	this->reporters.push_back(std::unique_ptr<tools::Reporter_BFER<B>>(reporter_BFER));
	auto reporter_thr = new tools::Reporter_throughput<uint64_t>(*this->monitor_er_red);
	this->reporters.push_back(std::unique_ptr<tools::Reporter_throughput<uint64_t>>(reporter_thr));
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::build_monitors()
{
	// build a monitor to compute BER/FER on each thread
	this->add_module("monitor_er", params_BFER.n_threads);
	for (auto tid = 0; tid < params_BFER.n_threads; tid++)
	{
		this->monitor_er[tid] = this->build_monitor_er(tid);
		this->set_module("monitor_er", tid, this->monitor_er[tid]);
	}

	// build a monitor to reduce BER/FER from the other monitors
	this->monitor_er_red.reset(new Monitor_BFER_reduction_type(this->monitor_er));

	if (params_BFER.mnt_mutinfo)
	{
		// build a monitor to compute MIon each thread
		this->add_module("monitor_mi", params_BFER.n_threads);
		for (auto tid = 0; tid < params_BFER.n_threads; tid++)
		{
			this->monitor_mi[tid] = this->build_monitor_mi(tid);
			this->set_module("monitor_mi", tid, this->monitor_mi[tid]);
		}

		// build a monitor to reduce M from the other monitors
		this->monitor_mi_red.reset(new Monitor_MI_reduction_type(this->monitor_mi));
	}

	module::Monitor_reduction::set_master_thread_id(std::this_thread::get_id());
#ifdef AFF3CT_MPI
	module::Monitor_reduction::set_reduce_frequency(params_BFER.mnt_mpi_comm_freq);
#else
	auto freq = std::chrono::milliseconds(0);
	if (params_BFER.mnt_red_lazy)
	{
		if (params_BFER.mnt_red_lazy_freq.count())
			freq = params_BFER.mnt_red_lazy_freq;
		else
			freq = std::chrono::milliseconds(1000); // default value when lazy reduction and no terminal refresh
	}
	module::Monitor_reduction::set_reduce_frequency(freq);
#endif

	module::Monitor_reduction::reset_all();
	module::Monitor_reduction::check_reducible();
}

template <typename B, typename R, typename Q>
void BFER<B,R,Q>
::start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->__build_communication_chain(tid);

		if (simu->params_BFER.err_track_enable)
			simu->monitor_er[tid]->add_handler_fe(std::bind(&tools::Dumper::add,
			                                                simu->dumper[tid].get(),
			                                                std::placeholders::_1,
			                                                std::placeholders::_2));
	}
	catch (std::exception const& e)
	{
		tools::Terminal::stop();
		simu->simu_error = true;

		simu->mutex_exception.lock();

		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		std::string msg = e.what(); // get only the function signature
		tools::exception::no_backtrace = save;

		if (std::find(simu->prev_err_messages.begin(), simu->prev_err_messages.end(), msg) ==
		                                               simu->prev_err_messages.end())
		{
			// with backtrace if debug mode
			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
			// save only the function signature
			simu->prev_err_messages.push_back(msg);
		}
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
bool BFER<B,R,Q>
::keep_looping_noise_point()
{
	// communication chain execution
	return !(tools::Terminal::is_interrupt() // if user stopped the simulation
	         || module::Monitor_reduction::is_done_all() // while any monitor criteria is not reached -> do reduction
	         || this->stop_time_reached());
}

template <typename B, typename R, typename Q>
bool BFER<B,R,Q>
::stop_time_reached()
{
	using namespace std::chrono;
	return params_BFER.stop_time != seconds(0) && (steady_clock::now() - this->t_start_noise_point) >=
	                                                                     params_BFER.stop_time;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::BFER<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
