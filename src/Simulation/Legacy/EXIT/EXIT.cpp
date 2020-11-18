#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#include <functional>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <sstream>
#include <chrono>
#include <limits>
#include <string>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Display/Statistics/Statistics.hpp"
#include "Tools/general_utils.h"
#include "Tools/Math/utils.h"
#include "Tools/Display/Reporter/EXIT/Reporter_EXIT.hpp"
#include "Tools/Display/Reporter/Noise/Reporter_noise.hpp"
#include "Tools/Display/Reporter/Throughput/Reporter_throughput.hpp"
#include "Simulation/Legacy/EXIT/EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R>
EXIT<B,R>
::EXIT(const factory::EXIT& params_EXIT)
: Simulation_legacy(params_EXIT         ),
  params_EXIT      (params_EXIT         ),
  noise_vals       (params_EXIT.n_frames),
  noise_a_vals     (params_EXIT.n_frames),
  sig_a            ((R)0                )
{
#ifdef AFF3CT_MPI
	std::clog << rang::tag::warning << "This simulation is not MPI ready, the same computations will be launched "
	                                   "on each MPI processes." << std::endl;
#endif

	if (params_EXIT.noise->type != "EBN0" && params_EXIT.noise->type != "ESN0")
	{
		std::stringstream message;
		message << "Wrong noise type, must be gaussian noise EBN0 or ESN0 ('params_EXIT.noise->type' = "
		        << params_EXIT.noise->type << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (params_EXIT.n_threads > 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "EXIT simu does not support the multi-threading.");

	try
	{
		auto cstl = params_EXIT.mdm->build_constellation<R>();
		constellation.reset(cstl);
	}
	catch(tools::cannot_allocate&) {}

	this->add_module("source"   , params_EXIT.n_threads);
	this->add_module("extractor", params_EXIT.n_threads);
	this->add_module("encoder"  , params_EXIT.n_threads);
	this->add_module("decoder"  , params_EXIT.n_threads);
	this->add_module("modem"    , params_EXIT.n_threads);
	this->add_module("modem_a"  , params_EXIT.n_threads);
	this->add_module("channel"  , params_EXIT.n_threads);
	this->add_module("channel_a", params_EXIT.n_threads);
	this->add_module("monitor"  , params_EXIT.n_threads);

	this->monitor = this->build_monitor();

	this->set_module("monitor", 0, *this->monitor);

	auto reporter_noise = new tools::Reporter_noise<>(this->noise);
	reporters.push_back(std::unique_ptr<tools::Reporter_noise<>>(reporter_noise));
	auto reporter_EXIT = new tools::Reporter_EXIT<B,R>(*this->monitor, this->noise_a);
	reporters.push_back(std::unique_ptr<tools::Reporter_EXIT<B,R>>(reporter_EXIT));
	auto reporter_thr = new tools::Reporter_throughput<uint64_t>(*this->monitor);
	reporters.push_back(std::unique_ptr<tools::Reporter_throughput<uint64_t>>(reporter_thr));
}

template <typename B, typename R>
void EXIT<B,R>
::_build_communication_chain()
{
	const auto N_mod = params_EXIT.mdm->N_mod;
	const auto K_mod = factory::Modem::get_buffer_size_after_modulation(params_EXIT.mdm->type,
	                                                                    params_EXIT.cdc->K,
	                                                                    params_EXIT.mdm->bps,
	                                                                    params_EXIT.mdm->cpm_upf,
	                                                                    params_EXIT.mdm->cpm_L);

	// build the objects
	source    = build_source   (     );
	codec     = build_codec    (     );
	modem     = build_modem    (     );
	modem_a   = build_modem_a  (     );
	channel   = build_channel  (N_mod);
	channel_a = build_channel_a(K_mod);
	terminal  = build_terminal (     );

	this->set_module("source"   , 0, *source);
	this->set_module("extractor", 0,  codec->get_extractor());
	this->set_module("encoder"  , 0,  codec->get_encoder());
	this->set_module("decoder"  , 0,  codec->get_decoder_siso());
	this->set_module("modem"    , 0, *modem);
	this->set_module("modem_a"  , 0, *modem_a);
	this->set_module("channel"  , 0, *channel);
	this->set_module("channel_a", 0, *channel_a);

	if (codec->get_decoder_siso().get_n_frames() > 1)
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "The inter frame is not supported.");
}

template <typename B, typename R>
void EXIT<B,R>
::launch()
{
	this->terminal = this->build_terminal();

	// allocate and build all the communication chain to generate EXIT chart
	this->build_communication_chain();
	this->sockets_binding();

	// for each channel NOISE to be simulated
	for (unsigned noise_idx = 0; noise_idx < params_EXIT.noise->range.size(); noise_idx ++)
	{
		R ebn0 = params_EXIT.noise->range[noise_idx];

		// For EXIT simulation, NOISE is considered as Es/N0
		const R bit_rate = 1.;
		R esn0  = tools::ebn0_to_esn0 (ebn0, bit_rate, params_EXIT.mdm->bps);
		R sigma = tools::esn0_to_sigma(esn0, params_EXIT.mdm->cpm_upf);

		this->noise.set_values(sigma, ebn0, esn0);

		std::fill(this->noise_vals.begin(), this->noise_vals.end(), this->noise.get_value());

		// for each "a" standard deviation (sig_a) to be simulated
		using namespace module;
		for (unsigned sig_a_idx = 0; sig_a_idx < params_EXIT.sig_a_range.size(); sig_a_idx ++)
		{
			sig_a = params_EXIT.sig_a_range[sig_a_idx];

			if (sig_a == 0.f) // if sig_a = 0, La_K2 = 0
			{
				auto &mdm = *this->modem_a;
				if (params_EXIT.chn->type.find("RAYLEIGH") != std::string::npos)
				{
					auto mdm_data  = (uint8_t*)(mdm[mdm::sck::demodulate_wg::Y_N2].get_dataptr());
					auto mdm_bytes =            mdm[mdm::sck::demodulate_wg::Y_N2].get_databytes();
					std::fill(mdm_data, mdm_data + mdm_bytes, 0);
				}
				else
				{
					auto mdm_data  = (uint8_t*)(mdm[mdm::sck::demodulate::Y_N2].get_dataptr());
					auto mdm_bytes =            mdm[mdm::sck::demodulate::Y_N2].get_databytes();
					std::fill(mdm_data, mdm_data + mdm_bytes, 0);
				}
				this->noise_a.set_value(std::numeric_limits<R>::infinity());
			}
			else
			{
				const R bit_rate = 1.;
				auto sig_a_2 = (R)2. / sig_a;
				R sig_a_esn0 = tools::sigma_to_esn0(sig_a_2, params_EXIT.mdm->cpm_upf);
				R sig_a_ebn0 = tools::esn0_to_ebn0 (sig_a_esn0, bit_rate, params_EXIT.mdm->bps);

				this->noise_a.set_values(sig_a_2, sig_a_ebn0, sig_a_esn0);
			}

			std::fill(this->noise_a_vals.begin(), this->noise_a_vals.end(), this->noise_a.get_value());

			if ((!params_EXIT.ter->disabled && noise_idx == 0 && sig_a_idx == 0 && !params_EXIT.debug)
				|| (params_EXIT.statistics && !params_EXIT.debug))
				terminal->legend(std::cout);

			// start the terminal to display BER/FER results
			if (!params_EXIT.ter->disabled && params_EXIT.ter->frequency != std::chrono::nanoseconds(0) &&
				!params_EXIT.debug)
				this->terminal->start_temp_report(params_EXIT.ter->frequency);

			this->simulation_loop();

			if (!params_EXIT.ter->disabled)
			{
				if (params_EXIT.debug)
					terminal->legend(std::cout);

				terminal->final_report(std::cout);

				if (params_EXIT.statistics)
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

			this->monitor->reset();
			for (auto &m : modules)
				for (auto& mm : m.second)
					if (mm != nullptr)
						for (auto &t : mm->tasks)
							t->reset();

			if (tools::Terminal::is_over())
				break;
		}

		if (tools::Terminal::is_over())
			break;
	}
}

template <typename B, typename R>
void EXIT<B,R>
::sockets_binding()
{
	auto &src = *this->source;
	auto &ext =  this->codec->get_extractor();
	auto &enc =  this->codec->get_encoder();
	auto &dec =  this->codec->get_decoder_siso();
	auto &mdm = *this->modem;
	auto &mda = *this->modem_a;
	auto &chn = *this->channel;
	auto &cha = *this->channel_a;
	auto &mnt = *this->monitor;

	using namespace module;

	mnt[mnt::sck::check_mutual_info::bits](src[src::sck::generate::U_K]);
	mda[mdm::sck::modulate         ::X_N1](src[src::sck::generate::U_K]);
	enc[enc::sck::encode           ::U_K ](src[src::sck::generate::U_K]);
	mdm[mdm::sck::modulate         ::X_N1](enc[enc::sck::encode  ::X_N]);

	// Rayleigh channel
	if (params_EXIT.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		cha[chn::sck::add_noise_wg ::noise].bind(this->noise_a_vals);
		mda[mdm::sck::demodulate_wg::noise].bind(this->noise_a_vals);
		cha[chn::sck::add_noise_wg ::X_N  ](mda[mdm::sck::modulate    ::X_N2]);
		mda[mdm::sck::demodulate_wg::H_N  ](cha[chn::sck::add_noise_wg::H_N ]);
		mda[mdm::sck::demodulate_wg::Y_N1 ](cha[chn::sck::add_noise_wg::Y_N ]);
	}
	else // additive channel (AWGN, USER, NO)
	{
		cha[chn::sck::add_noise ::noise].bind(this->noise_a_vals);
		mda[mdm::sck::demodulate::noise].bind(this->noise_a_vals);
		cha[chn::sck::add_noise ::X_N  ](mda[mdm::sck::modulate ::X_N2]);
		mda[mdm::sck::demodulate::Y_N1 ](cha[chn::sck::add_noise::Y_N ]);
	}

	// Rayleigh channel
	if (params_EXIT.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		chn[chn::sck::add_noise_wg       ::noise ].bind(this->noise_vals);
		mdm[mdm::sck::demodulate_wg      ::noise ].bind(this->noise_vals);
		mnt[mnt::sck::check_mutual_info  ::llrs_a](mda[mdm::sck::demodulate_wg::Y_N2]);
		chn[chn::sck::add_noise_wg       ::X_N   ](mdm[mdm::sck::modulate     ::X_N2]);
		mdm[mdm::sck::demodulate_wg      ::H_N   ](chn[chn::sck::add_noise_wg ::H_N ]);
		mdm[mdm::sck::demodulate_wg      ::Y_N1  ](chn[chn::sck::add_noise_wg ::Y_N ]);
		ext[ext::sck::add_sys_and_ext_llr::ext   ](mda[mdm::sck::demodulate_wg::Y_N2]);
		ext[ext::sck::add_sys_and_ext_llr::Y_N   ](mdm[mdm::sck::demodulate_wg::Y_N2]);
		dec[dec::sck::decode_siso        ::Y_N1  ](mdm[mdm::sck::demodulate_wg::Y_N2]);
	}
	else // additive channel (AWGN, USER, NO)
	{
		chn[chn::sck::add_noise          ::noise ].bind(this->noise_vals);
		mdm[mdm::sck::demodulate         ::noise ].bind(this->noise_vals);
		mnt[mnt::sck::check_mutual_info  ::llrs_a](mda[mdm::sck::demodulate::Y_N2]);
		chn[chn::sck::add_noise          ::X_N   ](mdm[mdm::sck::modulate  ::X_N2]);
		mdm[mdm::sck::demodulate         ::Y_N1  ](chn[chn::sck::add_noise ::Y_N ]);
		ext[ext::sck::add_sys_and_ext_llr::ext   ](mda[mdm::sck::demodulate::Y_N2]);
		ext[ext::sck::add_sys_and_ext_llr::Y_N   ](mdm[mdm::sck::demodulate::Y_N2]);
		dec[dec::sck::decode_siso        ::Y_N1  ](mdm[mdm::sck::demodulate::Y_N2]);
	}

	ext[ext::sck::get_sys_llr      ::Y_N   ](dec[dec::sck::decode_siso::Y_N2]);
	mnt[mnt::sck::check_mutual_info::llrs_e](ext[ext::sck::get_sys_llr::Y_K ]);
}

template <typename B, typename R>
void EXIT<B,R>
::simulation_loop()
{
	auto &source    = *this->source;
	auto &extractor =  this->codec->get_extractor();
	auto &encoder   =  this->codec->get_encoder();
	auto &decoder   =  this->codec->get_decoder_siso();
	auto &modem     = *this->modem;
	auto &modem_a   = *this->modem_a;
	auto &channel   = *this->channel;
	auto &channel_a = *this->channel_a;
	auto &monitor   = *this->monitor;

	using namespace module;

	while (!monitor.n_trials_achieved())
	{
		if (params_EXIT.debug)
		{
			if (!monitor[mnt::tsk::check_mutual_info].get_n_calls())
				std::cout << "#" << std::endl;

			auto fid = monitor[mnt::tsk::check_mutual_info].get_n_calls();
			std::cout << "# -------------------------------"     << std::endl;
			std::cout << "# New communication (n°" << fid << ")" << std::endl;
			std::cout << "# -------------------------------"     << std::endl;
			std::cout << "#"                                     << std::endl;
		}

		source [src::tsk::generate].exec();
		modem_a[mdm::tsk::modulate].exec();
		encoder[enc::tsk::encode  ].exec();
		modem  [mdm::tsk::modulate].exec();

		//if sig_a = 0, La_K = 0, no noise to add
		if (sig_a != (R)0.)
		{
			// Rayleigh channel
			if (params_EXIT.chn->type.find("RAYLEIGH") != std::string::npos)
			{
				channel_a[chn::tsk::add_noise_wg ].exec();
				modem_a  [mdm::tsk::demodulate_wg].exec();
			}
			else // additive channel (AWGN, USER, NO)
			{
				channel_a[chn::tsk::add_noise ].exec();
				modem_a  [mdm::tsk::demodulate].exec();
			}
		}

		// Rayleigh channel
		if (params_EXIT.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			channel[chn::tsk::add_noise_wg ].exec();
			modem  [mdm::tsk::demodulate_wg].exec();
		}
		else // additive channel (AWGN, USER, NO)
		{
			channel[chn::tsk::add_noise ].exec();
			modem  [mdm::tsk::demodulate].exec();
		}

		extractor[ext::tsk::add_sys_and_ext_llr].exec();
		decoder  [dec::tsk::decode_siso        ].exec();
		extractor[ext::tsk::get_sys_llr        ].exec();
		monitor  [mnt::tsk::check_mutual_info  ].exec();
	}
}

template <typename B, typename R>
std::unique_ptr<module::Source<B>> EXIT<B,R>
::build_source()
{
	auto src = std::unique_ptr<module::Source<B>>(params_EXIT.src->template build<B>());
	src->set_n_frames(this->params.n_frames);
	return src;
}

template <typename B, typename R>
std::unique_ptr<tools::Codec_SISO<B,R>> EXIT<B,R>
::build_codec()
{
	auto cdc = std::unique_ptr<tools::Codec_SISO<B,R>>(params_EXIT.cdc->template build<B,R>());
	cdc->set_n_frames(this->params.n_frames);
	cdc->set_noise(this->noise);

	auto ptr = cdc.get();
	this->noise.record_callback_update([ptr]() { ptr->notify_noise_update(); });

	return cdc;
}

template <typename B, typename R>
std::unique_ptr<module::Modem<B,R,R>> EXIT<B,R>
::build_modem()
{
	auto mdm = std::unique_ptr<module::Modem<B,R,R>>(params_EXIT.mdm->template build<B,R>(this->constellation.get()));
	mdm->set_n_frames(this->params.n_frames);

	return mdm;
}

template <typename B, typename R>
std::unique_ptr<module::Modem<B,R>> EXIT<B,R>
::build_modem_a()
{
	std::unique_ptr<factory::Modem> mdm_params(params_EXIT.mdm->clone());
	mdm_params->N = params_EXIT.cdc->K;
	auto mdm = std::unique_ptr<module::Modem<B,R>>(mdm_params->template build<B,R>(this->constellation.get()));
	mdm->set_n_frames(this->params.n_frames);

	return mdm;
}

template <typename B, typename R>
std::unique_ptr<module::Channel<R>> EXIT<B,R>
::build_channel(const int size)
{
	auto chn = std::unique_ptr<module::Channel<R>>(params_EXIT.chn->template build<R>());
	chn->set_n_frames(this->params.n_frames);

	return chn;
}

template <typename B, typename R>
std::unique_ptr<module::Channel<R>> EXIT<B,R>
::build_channel_a(const int size)
{
	std::unique_ptr<factory::Channel> chn_params(params_EXIT.chn->clone());
	chn_params->N = factory::Modem::get_buffer_size_after_modulation(params_EXIT.mdm->type,
	                                                                 params_EXIT.cdc->K,
	                                                                 params_EXIT.mdm->bps,
	                                                                 params_EXIT.mdm->cpm_upf,
	                                                                 params_EXIT.mdm->cpm_L);

	auto chn = std::unique_ptr<module::Channel<R>>(chn_params->template build<R>());
	chn->set_n_frames(this->params.n_frames);

	return chn;
}

template <typename B, typename R>
std::unique_ptr<module::Monitor_EXIT<B,R>> EXIT<B,R>
::build_monitor()
{
	auto mnt = std::unique_ptr<module::Monitor_EXIT<B,R>>(params_EXIT.mnt->template build<B,R>());
	mnt->set_n_frames(this->params.n_frames);
	return mnt;
}

template <typename B, typename R>
std::unique_ptr<tools::Terminal> EXIT<B,R>
::build_terminal()
{
	return std::unique_ptr<tools::Terminal>(params_EXIT.ter->build(this->reporters));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::EXIT<B_32,R_32>;
template class aff3ct::simulation::EXIT<B_64,R_64>;
#else
template class aff3ct::simulation::EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation

#endif