#include <cmath>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/general_utils.h"
#include "Tools/Math/utils.h"

#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "EXIT.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R>
EXIT<C,B,R>
::EXIT(const factory::EXIT::parameters<C>& params)
: Simulation(),
  params(params),

  H_N   (params.cdc.dec.N_cw),
  B_K   (params.cdc.dec.K   ),
  B_N   (params.cdc.dec.N_cw),
  X_N1  (params.cdc.dec.N_cw),
  X_K   (params.cdc.dec.K   ),
  X_N2  (params.cdc.dec.N_cw),
  Y_N   (params.cdc.dec.N_cw),
  Y_K   (params.cdc.dec.K   ),
  La_K1 (params.cdc.dec.K   ),
  Lch_N1(params.cdc.dec.N_cw),
  La_K2 (params.cdc.dec.K   ),
  Lch_N2(params.cdc.dec.N_cw),
  Le_K  (params.cdc.dec.K   ),
  sys   (params.cdc.dec.K                         +  params.cdc.dec.tail_length / 2),
  par   ((params.cdc.dec.N_cw - params.cdc.dec.K) - (params.cdc.dec.tail_length / 2)),

  sig_a(0.f),
  sigma(0.f),
  ebn0 (0.f),
  esn0 (0.f),

  source   (nullptr),
  codec    (nullptr),
  modem    (nullptr),
  modem_a  (nullptr),
  channel  (nullptr),
  channel_a(nullptr),
  siso     (nullptr),
  monitor  (nullptr),
  terminal (nullptr)
{
#ifdef ENABLE_MPI
	std::clog << tools::format_warning("This simulation is not MPI ready, the same computations will be launched "
	                                   "on each MPI processes.") << std::endl;
#endif

	this->monitor = this->build_monitor();
}

template <class C, typename B, typename R>
EXIT<C,B,R>
::~EXIT()
{
	if (monitor != nullptr) { delete monitor; monitor = nullptr; }
	release_objects();
}

template <class C, typename B, typename R>
void EXIT<C,B,R>
::build_communication_chain()
{
	release_objects();

	const auto N_mod = params.mdm.N_mod;
	const auto K_mod = factory::Modem::get_buffer_size_after_modulation(params.mdm.type,
	                                                                    params.cdc.enc.K,
	                                                                    params.mdm.bps,
	                                                                    params.mdm.upf,
	                                                                    params.mdm.cpm_L);

	// build the objects
	source    = build_source   (     );
	codec     = build_codec    (     );
	modem     = build_modem    (     );
	modem_a   = build_modem_a  (     );
	channel   = build_channel  (N_mod);
	channel_a = build_channel_a(K_mod);
	terminal  = build_terminal (     );

	if (codec->get_decoder_siso()->get_n_frames() > 1)
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "The inter frame is not supported.");

	if (X_K   .size() != (unsigned)K_mod) X_K   .resize(K_mod);
	if (X_N2  .size() != (unsigned)N_mod) X_N2  .resize(N_mod);
	if (La_K1 .size() != (unsigned)K_mod) La_K1 .resize(K_mod);
	if (Lch_N1.size() != (unsigned)N_mod) Lch_N1.resize(N_mod);
	if (H_N   .size() != (unsigned)N_mod) H_N   .resize(N_mod);
}

template <class C, typename B, typename R>
void EXIT<C,B,R>
::launch()
{
	// allocate and build all the communication chain to generate EXIT chart
	this->build_communication_chain();

	if (!params.ter.disabled)
		terminal->legend(std::cout);

	// for each channel SNR to be simulated	
	for (ebn0 = params.snr_min; ebn0 <= params.snr_max; ebn0 += params.snr_step)
	{
		// For EXIT simulation, SNR is considered as Es/N0
		const auto bit_rate = 1.f;
		esn0  = tools::ebn0_to_esn0 (ebn0, bit_rate, params.mdm.bps);
		sigma = tools::esn0_to_sigma(esn0, params.mdm.upf);

		terminal->set_esn0(esn0);
		terminal->set_ebn0(ebn0);

		channel->set_sigma(sigma);
		modem  ->set_sigma(sigma);
		codec  ->set_sigma(sigma);

		// for each "a" standard deviation (sig_a) to be simulated
		for (sig_a = params.sig_a_min; sig_a <= params.sig_a_max; sig_a += params.sig_a_step)
		{
			terminal ->set_sig_a(sig_a      );
			channel_a->set_sigma(2.f / sig_a);
			modem_a  ->set_sigma(2.f / sig_a);

			if (sig_a == 0.f) // if sig_a = 0, La_K2 = 0
				std::fill(La_K2.begin(), La_K2.end(), (R)0);

			this->simulation_loop();

			if (!params.ter.disabled)
				terminal->final_report(std::cout);

			this->monitor->reset();

			if (module::Monitor::is_over())
				break;
		}

		if (module::Monitor::is_over())
			break;
	}
}

template <class C, typename B, typename R>
void EXIT<C,B,R>
::simulation_loop()
{
	using namespace std::chrono;
	auto t_simu = steady_clock::now();

	auto *encoder      = codec->get_encoder();
	auto *decoder_siso = codec->get_decoder_siso();

	while (!monitor->n_trials_achieved())
	{
		// generate a random binary value
		source->generate(B_K);

		// encode
		encoder->encode(B_K, X_N1);

		// modulate
		modem_a->modulate(B_K,  X_K );
		modem  ->modulate(X_N1, X_N2);

		//if sig_a = 0, La_K = 0, no noise to add
		if (sig_a != 0)
		{
			// Rayleigh channel
			if (params.chn.type.find("RAYLEIGH") != std::string::npos)
			{
				channel_a->add_noise_wg (X_K, La_K1, H_N       );
				modem_a  ->demodulate_wg(     La_K1, H_N, La_K2);
			}
			else // additive channel (AWGN, USER, NO)
			{
				channel_a->add_noise (X_K, La_K1       );
				modem_a  ->demodulate(     La_K1, La_K2);
			}
		}

		// Rayleigh channel
		if (params.chn.type.find("RAYLEIGH") != std::string::npos)
		{
			channel->add_noise_wg (X_N2, Lch_N1, H_N        );
			modem  ->demodulate_wg(      Lch_N1, H_N, Lch_N2);
		}
		else // additive channel (AWGN, USER, NO)
		{
			channel->add_noise (X_N2, Lch_N1        );
			modem  ->demodulate(      Lch_N1, Lch_N2);
		}

		// extract systematic and parity information
		codec->extract_sys_par(Lch_N2, sys, par);

		// add other siso's extrinsic
		for (auto k = 0; k < params.cdc.enc.K; k++)
			sys[k] += La_K2[k];

		// decode
		decoder_siso->decode_siso(sys, par, Le_K);
		decoder_siso->reset();

		// compute the mutual info
		monitor->measure_mutual_info(B_K, La_K2, Le_K);

		// display statistics in terminal
		if (!params.ter.disabled && (steady_clock::now() - t_simu) >= params.ter.frequency)
		{
			terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}
}

template <class C, typename B, typename R>
void EXIT<C,B,R>
::release_objects()
{
	if (source    != nullptr) { delete source;    source    = nullptr; }
	if (codec     != nullptr) { delete codec;     codec     = nullptr; }
	if (modem     != nullptr) { delete modem;     modem     = nullptr; }
	if (modem_a   != nullptr) { delete modem_a;   modem_a   = nullptr; }
	if (channel   != nullptr) { delete channel;   channel   = nullptr; }
	if (channel_a != nullptr) { delete channel_a; channel_a = nullptr; }
	if (terminal  != nullptr) { delete terminal;  terminal  = nullptr; }
}

template <class C, typename B, typename R>
module::Source<B>* EXIT<C,B,R>
::build_source()
{
	return factory::Source::build<B>(params.src);
}

template <class C, typename B, typename R>
module::Codec_SISO<B,R>* EXIT<C,B,R>
::build_codec()
{
	return params.cdc.template build<B,R>();
}

template <class C, typename B, typename R>
module::Modem<B,R,R>* EXIT<C,B,R>
::build_modem()
{
	return factory::Modem::build<B,R>(params.mdm);
}

template <class C, typename B, typename R>
module::Modem<B,R,R>* EXIT<C,B,R>
::build_modem_a()
{
	auto mdm_params = params.mdm;
	mdm_params.N    = params.cdc.enc.K;
	return factory::Modem::build<B,R>(mdm_params);
}

template <class C, typename B, typename R>
module::Channel<R>* EXIT<C,B,R>
::build_channel(const int size)
{
	return factory::Channel::build<R>(params.chn);
}

template <class C, typename B, typename R>
module::Channel<R>* EXIT<C,B,R>
::build_channel_a(const int size)
{
	auto chn_params = params.chn;
	chn_params.N    = factory::Modem::get_buffer_size_after_modulation(params.mdm.type,
	                                                                   params.cdc.enc.K,
	                                                                   params.mdm.bps,
	                                                                   params.mdm.upf,
 	                                                                   params.mdm.cpm_L);
	return factory::Channel::build<R>(chn_params);
}

template <class C, typename B, typename R>
module::Monitor_EXIT<B,R>* EXIT<C,B,R>
::build_monitor()
{
	return factory::Monitor_EXIT::build<B,R>(params.mnt);
}

template <class C, typename B, typename R>
tools::Terminal_EXIT<B,R>* EXIT<C,B,R>
::build_terminal()
{
	return factory::Terminal_EXIT::build<B,R>(params.ter, *this->monitor);
}
}
}
