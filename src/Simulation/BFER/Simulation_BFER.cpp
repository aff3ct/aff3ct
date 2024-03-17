#include <exception>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <random>
#include <thread>
#include <string>
#include <ios>

#include "Tools/Reporter/MI/Reporter_MI.hpp"
#include "Tools/Reporter/BFER/Reporter_BFER.hpp"
#include "Tools/Reporter/Noise/Reporter_noise.hpp"
#include "Tools/Reporter/Throughput/Reporter_throughput.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Display/Statistics/Statistics.hpp"
#include "Tools/Exception/exception.hpp"
#include "Simulation/BFER/Simulation_BFER.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R>
Simulation_BFER<B,R>
::Simulation_BFER(const factory::BFER &params_BFER)
: Simulation(),
  params(params_BFER),
  params_BFER(params_BFER),
  noise(params_BFER.noise->build<>()),
  channel_params(params_BFER.n_frames),
  dumper(params_BFER.n_threads)
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
		this->distributions.reset(new tools::Distributions<R>(params_BFER.noise->pdf_path,
		                                                      tools::Distribution_mode::SUMMATION));
	try
	{
		this->constellation.reset(params_BFER.mdm->build_constellation<R>());
	}
	catch (tools::cannot_allocate&) {}
}

template <typename B, typename R>
std::unique_ptr<module::Monitor_MI<B,R>> Simulation_BFER<B,R>
::build_monitor_mi()
{
	auto mnt = std::unique_ptr<module::Monitor_MI<B,R>>(params_BFER.mnt_mi->build<B,R>());
	mnt->set_n_frames(this->params.n_frames);
	return mnt;
}

template <typename B, typename R>
std::unique_ptr<module::Monitor_BFER<B>> Simulation_BFER<B,R>
::build_monitor_er()
{
	bool count_unknown_values = params_BFER.noise->type == "EP";

	auto mnt_tmp = params_BFER.mnt_er->build<B>(count_unknown_values);
	auto mnt = std::unique_ptr<module::Monitor_BFER<B>>(mnt_tmp);
	mnt->activate_err_histogram(params_BFER.mnt_er->err_hist != -1);

	mnt->set_n_frames(this->params.n_frames);

	return mnt;
}

template <typename B, typename R>
std::vector<std::unique_ptr<tools::Reporter>> Simulation_BFER<B,R>
::build_reporters(const tools ::Noise       < > *noise,
	              const module::Monitor_BFER<B> *monitor_er,
	              const module::Monitor_MI<B,R> *monitor_mi)
{
	std::vector<std::unique_ptr<tools::Reporter>> reporters;
	auto reporter_noise = new tools::Reporter_noise<>(*noise, this->params_BFER.ter_sigma);
	reporters.push_back(std::unique_ptr<tools::Reporter_noise<>>(reporter_noise));

	if (params_BFER.mnt_mutinfo)
	{
		auto reporter_MI = new tools::Reporter_MI<B,R>(*monitor_mi);
		reporters.push_back(std::unique_ptr<tools::Reporter_MI<B,R>>(reporter_MI));
	}

	auto reporter_BFER = new tools::Reporter_BFER<B>(*monitor_er);
	reporters.push_back(std::unique_ptr<tools::Reporter_BFER<B>>(reporter_BFER));
	auto reporter_thr = new tools::Reporter_throughput<uint64_t>(*monitor_er);
	reporters.push_back(std::unique_ptr<tools::Reporter_throughput<uint64_t>>(reporter_thr));

	return reporters;
}

template <typename B, typename R>
std::unique_ptr<tools::Terminal> Simulation_BFER<B,R>
::build_terminal(const std::vector<std::unique_ptr<tools::Reporter>> &reporters)
{
	return std::unique_ptr<tools::Terminal>(params_BFER.ter->build(reporters));
}

template <typename B, typename R>
void Simulation_BFER<B,R>
::create_modules()
{
	this->monitor_er = this->build_monitor_er();
	if (params_BFER.mnt_mi != nullptr)
		this->monitor_mi = this->build_monitor_mi();
}

template <typename B, typename R>
void Simulation_BFER<B,R>
::configure_sequence_tasks()
{
	if (!params_BFER.sequence_path.empty())
	{
		std::ofstream dot_file(params_BFER.sequence_path);
		this->sequence->export_dot(dot_file);
	}

	for (auto &mod : sequence->get_modules<module::Module>())
		for (auto &tsk : mod->tasks)
		{
			if (this->params.statistics)
				tsk->set_stats(true);
			// enable the debug mode in the modules
			if (this->params.debug)
			{
				tsk->set_debug(true);
				tsk->set_debug_hex(this->params.debug_hex);
				if (this->params.debug_limit)
					tsk->set_debug_limit((uint32_t)this->params.debug_limit);
				if (this->params.debug_precision)
					tsk->set_debug_precision((uint8_t)this->params.debug_precision);
				if (params.debug_frame_max)
					tsk->set_debug_frame_max((uint32_t)this->params.debug_frame_max);
			}
			// if (!tsk->is_stats() && !tsk->is_debug())
				tsk->set_fast(true);
		}
}

template <typename B, typename R>
void Simulation_BFER<B,R>
::create_monitors_reduction()
{
	auto monitors_bfer = sequence->get_modules<module::Monitor_BFER<B>>();
#ifdef AFF3CT_MPI
	this->monitor_er_red.reset(new tools::Monitor_reduction_MPI<module::Monitor_BFER<B>>(monitors_bfer));
#else
	this->monitor_er_red.reset(new tools::Monitor_reduction<module::Monitor_BFER<B>>(monitors_bfer));
#endif

	if (params_BFER.mnt_mutinfo)
	{
		auto monitors_mi = sequence->get_modules<module::Monitor_MI<B,R>>();
#ifdef AFF3CT_MPI
		this->monitor_mi_red.reset(new tools::Monitor_reduction_MPI<module::Monitor_MI<B,R>>(monitors_mi));
#else
		this->monitor_mi_red.reset(new tools::Monitor_reduction<module::Monitor_MI<B,R>>(monitors_mi));
#endif
	}

	tools::Monitor_reduction_static::set_master_thread_id(std::this_thread::get_id());
#ifdef AFF3CT_MPI
	tools::Monitor_reduction_static::set_reduce_frequency(params_BFER.mnt_mpi_comm_freq);
#else
	auto freq = std::chrono::milliseconds(0);
	if (params_BFER.mnt_red_lazy)
	{
		if (params_BFER.mnt_red_lazy_freq.count())
			freq = params_BFER.mnt_red_lazy_freq;
		else
			freq = std::chrono::milliseconds(1000); // default value when lazy reduction and no terminal refresh
	}
	tools::Monitor_reduction_static::set_reduce_frequency(freq);
#endif
	tools::Monitor_reduction_static::reset_all();
	tools::Monitor_reduction_static::check_reducible();
}

template <typename B, typename R>
void Simulation_BFER<B,R>
::launch()
{
	if (!params_BFER.err_track_revert)
	{
		this->create_modules();
		this->bind_sockets();
		this->create_sequence();
		this->configure_sequence_tasks();
		this->create_monitors_reduction();

		this->reporters = this->build_reporters(this->noise.get(),
		                                        this->monitor_er_red.get(),
		                                        this->monitor_mi_red.get());
		this->terminal = this->build_terminal(this->reporters);
	}

	int noise_begin = 0;
	int noise_end   = (int)params_BFER.noise->range.size();
	int noise_step  = 1;
	if (params_BFER.noise->type == "EP")
	{
		noise_begin = (int)params_BFER.noise->range.size() -1;
		noise_end   = -1;
		noise_step  = -1;
	}

	// for each NOISE to be simulated
	for (auto noise_idx = noise_begin; noise_idx != noise_end; noise_idx += noise_step)
	{
		auto bit_rate = (float)params_BFER.src->K / (float)params_BFER.cdc->N;
		params_BFER.noise->template update<>(*this->noise,
		                                     params_BFER.noise->range[noise_idx],
		                                     bit_rate,
		                                     params_BFER.mdm->bps,
		                                     params_BFER.mdm->cpm_upf);

		std::fill(this->channel_params.begin(), this->channel_params.end(), this->noise->get_value());

		if (params_BFER.err_track_revert)
		{
			// dirty hack to override simulation params_BFER
			auto &params_BFER_writable = const_cast<factory::BFER&>(params_BFER);

			std::stringstream s_noise;
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_value();

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

			this->noise->clear_callbacks_update();

			this->create_modules();
			this->bind_sockets();
			this->create_sequence();
			this->configure_sequence_tasks();
			this->create_monitors_reduction();

			this->reporters = this->build_reporters(this->noise.get(),
			                                        this->monitor_er_red.get(),
			                                        this->monitor_mi_red.get());
			this->terminal = this->build_terminal(this->reporters);
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
			this->sequence->exec([this]() { return this->stop_condition(); } );
			tools::Monitor_reduction_static::last_reduce_all(); // final reduction
		}
		catch (std::exception const& e)
		{
			tools::Monitor_reduction_static::last_reduce_all(); // final reduction

			terminal->final_report(std::cout); // display final report to not lost last line overwritten by the error
			                                   // messages
			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
			this->simu_error = true;
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
				std::cout << "#" << std::endl;
				tools::Stats::show(this->sequence->get_modules_per_types(), true, true, std::cout);
				std::cout << "#" << std::endl;
			}
		}

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
						noise_value = std::to_string(this->noise->get_value());
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
			s_noise << std::setprecision(2) << std::fixed << this->noise->get_value();

			this->dumper_red->dump(params_BFER.err_track_path + "_" + s_noise.str());
			this->dumper_red->clear();
		}

		for (auto &mod : sequence->get_modules<module::Module>())
			for (auto &tsk : mod->tasks)
				tsk->reset();

		tools::Monitor_reduction_static::reset_all();
	}
}

template <typename B, typename R>
bool Simulation_BFER<B,R>
::stop_time_reached()
{
	return this->params_BFER.stop_time != std::chrono::seconds(0) &&
	       (std::chrono::steady_clock::now() - this->t_start_noise_point) >= this->params_BFER.stop_time;
}

template <typename B, typename R>
bool Simulation_BFER<B,R>
::stop_condition()
{
	return tools::Monitor_reduction_static::is_done_all() || stop_time_reached();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::Simulation_BFER<B_8,R_8>;
template class aff3ct::simulation::Simulation_BFER<B_16,R_16>;
template class aff3ct::simulation::Simulation_BFER<B_32,R_32>;
template class aff3ct::simulation::Simulation_BFER<B_64,R_64>;
#else
template class aff3ct::simulation::Simulation_BFER<B,R>;
#endif
// ==================================================================================== explicit template instantiation
