#include <cmath>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/general_utils.h"
#include "Tools/Math/utils.h"

#include "Factory/Module/Source.hpp"
#include "Factory/Module/Code/Encoder.hpp"
#include "Factory/Module/Modem.hpp"
#include "Factory/Module/Channel.hpp"
#include "Factory/Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R>
EXIT<B,R>
::EXIT(const factory::EXIT::parameters& params, Codec_SISO<B,R> &codec)
: Simulation(),
  params(params),
  codec (codec),

  H_N   (params.dec->N_cw),
  B_K   (params.dec->K   ),
  B_N   (params.dec->N_cw),
  X_N1  (params.dec->N_cw),
  X_K   (params.dec->K   ),
  X_N2  (params.dec->N_cw),
  Y_N   (params.dec->N_cw),
  Y_K   (params.dec->K   ),
  La_K1 (params.dec->K   ),
  Lch_N1(params.dec->N_cw),
  La_K2 (params.dec->K   ),
  Lch_N2(params.dec->N_cw),
  Le_K  (params.dec->K   ),
  sys   (params.dec->K                      +  params.dec->tail_length / 2),
  par   ((params.dec->N_cw - params.dec->K) - (params.dec->tail_length / 2)),

  B_buff (0),
  Le_buff(0),
  La_buff(0),

  n_trials (200000 / params.dec->K),
  cur_trial(0),

  I_A      (0.0),
  I_E      (0.0),
  sig_a    (0.f),
  sigma    (0.f),
  snr      (0.f),

  source   (nullptr),
  encoder  (nullptr),
  modem    (nullptr),
  modem_a  (nullptr),
  channel  (nullptr),
  channel_a(nullptr),
  siso     (nullptr),
  terminal (nullptr)
{
#ifdef ENABLE_MPI
	std::clog << format_warning("This simulation is not MPI ready, the same computations will be launched "
	                            "on each MPI processes.") << std::endl;
#endif
}

template <typename B, typename R>
EXIT<B,R>
::~EXIT()
{
	release_objects();
}

template <typename B, typename R>
void EXIT<B,R>
::build_communication_chain()
{
	release_objects();

	const auto N_mod = params.mdm->N_mod;
	const auto K_mod = factory::Modem::get_buffer_size_after_modulation(params.mdm->type,
	                                                                    params.enc->K,
	                                                                    params.mdm->bps,
	                                                                    params.mdm->upf,
	                                                                    params.mdm->cpm_L);

	// build the objects
	source    = build_source   (     );
	encoder   = build_encoder  (     );
	modem     = build_modem    (     );
	modem_a   = build_modem_a  (     );
	channel   = build_channel  (N_mod);
	channel_a = build_channel_a(K_mod);
	siso      = build_siso     (     );
	terminal  = build_terminal (     );

	if (siso->get_n_frames() > 1)
		throw runtime_error(__FILE__, __LINE__, __func__, "The inter frame is not supported.");

	if (X_K   .size() != (unsigned)K_mod) X_K   .resize(K_mod);
	if (X_N2  .size() != (unsigned)N_mod) X_N2  .resize(N_mod);
	if (La_K1 .size() != (unsigned)K_mod) La_K1 .resize(K_mod);
	if (Lch_N1.size() != (unsigned)N_mod) Lch_N1.resize(N_mod);
	if (H_N   .size() != (unsigned)N_mod) H_N   .resize(N_mod);
}

template <typename B, typename R>
void EXIT<B,R>
::launch()
{
	bool first_loop = true;

	codec.launch_precompute();

	// for each channel SNR to be simulated	
	for (snr = params.snr_min; snr <= params.snr_max; snr += params.snr_step)
	{
		// For EXIT simulation, SNR is considered as Es/N0
		const auto bit_rate = 1.f;
		sigma = esn0_to_sigma(ebn0_to_esn0(snr, bit_rate, params.mdm->bps), params.mdm->upf);

		codec.snr_precompute(sigma);

		// for each "a" standard deviation (sig_a) to be simulated
		for (sig_a = params.sig_a_min; sig_a <= params.sig_a_max; sig_a += params.sig_a_step)
		{
			I_A = 0.0;
			I_E = 0.0;

			// allocate and build all the communication chain to generate EXIT chart
			this->build_communication_chain();

			// if sig_a = 0, La_K2 = 0
			if (sig_a == 0)
				std::fill(La_K2.begin(), La_K2.end(), init_LLR<R>());

			if (!params.ter->disabled && first_loop)
			{
				terminal->legend(std::cout);
				first_loop = false;
			}

			this->simulation_loop();

			if (!params.ter->disabled)
				terminal->final_report(std::cout);
		}
	}
}

template <typename B, typename R>
void EXIT<B,R>
::simulation_loop()
{
	using namespace std::chrono;
	auto t_simu = steady_clock::now();

	Le_buff.clear();
	B_buff .clear();
	La_buff.clear();

	for (cur_trial = 0; cur_trial < n_trials; cur_trial++) 
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
			if (params.chn->type.find("RAYLEIGH") != std::string::npos)
			{
				channel_a->add_noise            (X_K, La_K1, H_N       );
				modem_a  ->demodulate_with_gains(     La_K1, H_N, La_K2);
			}
			else // additive channel (AWGN, USER, NO)
			{
				channel_a->add_noise (X_K, La_K1       );
				modem_a  ->demodulate(     La_K1, La_K2);
			}
		}

		// Rayleigh channel
		if (params.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			channel->add_noise            (X_N2, Lch_N1, H_N        );
			modem  ->demodulate_with_gains(      Lch_N1, H_N, Lch_N2);
		}
		else // additive channel (AWGN, USER, NO)
		{
			channel->add_noise (X_N2, Lch_N1        );
			modem  ->demodulate(      Lch_N1, Lch_N2);
		}

		// extract systematic and parity information
		codec.extract_sys_par(Lch_N2, sys, par);

		// add other siso's extrinsic
		for (auto k = 0; k < params.enc->K; k++)
			sys[k] += La_K2[k];

		// decode
		siso->decode_siso(sys, par, Le_K);

		// store B_K, La_K and Le_K in buffers (add current B and L to the buffers)
		B_buff .insert(B_buff .end(), B_K  .begin(), B_K  .end());
		Le_buff.insert(Le_buff.end(), Le_K .begin(), Le_K .end());
		La_buff.insert(La_buff.end(), La_K2.begin(), La_K2.end());

		// display statistics in terminal
		if (!params.ter->disabled && (steady_clock::now() - t_simu) >= params.ter->frequency)
		{
			terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}

	// measure mutual information and store it in I_A, I_E, sig_a_array
	I_A = EXIT<B,R>::measure_mutual_info_avg  (La_buff, B_buff) / (params.enc->K * n_trials);
	I_E = EXIT<B,R>::measure_mutual_info_histo(Le_buff, B_buff);
}

template <typename B, typename R>
double EXIT<B,R>
::measure_mutual_info_avg(const mipp::vector<R>& llrs, const mipp::vector<B>& bits)
{
	double I_A = 0;
	double symb;
	auto size = (int)llrs.size();
	for (int j = 0; j < size; j++)
	{
		symb = -2 * (double)bits[j] +1;
		I_A += (1 - std::log2(1 + std::exp(-symb * (double)llrs[j])));
	}

	return(I_A);
}

template <typename B, typename R>
double EXIT<B,R>
::measure_mutual_info_histo(const mipp::vector<R>& llrs, const mipp::vector<B>& bits)
{
//	const double inf = 10000000;
	const double inf = std::numeric_limits<double>::infinity();

	int vec_length = (int)bits.size();

	bool lots_of_bins;
	int bin_count, bin_offset = 0, bit_0_count, bit_1_count;
	int llr_0_noninfinite_count = 0;
	int llr_1_noninfinite_count = 0;

	double tmp;
	double I_E = 0.0;
	double bin_width = 0.0;
	double llr_0_variance = 0.0;
	double llr_1_variance = 0.0;
	double llr_0_max, llr_0_min, llr_1_max, llr_1_min;
	double llr_0_mean = 0.0;
	double llr_1_mean = 0.0;

	bit_1_count = 0;
	for (int i = 0; i < (int)bits.size(); i++)
		bit_1_count += (int)bits[i];

	bit_0_count = vec_length - bit_1_count;
	if (bit_0_count == 0 || bit_1_count == 0)
		I_E = 0;
	else
	{
		// determine the min and max value for llrs / 0 and llrs / 1
		llr_0_max = llr_1_max = -inf;
		llr_0_min = llr_1_min = +inf;

		for (int i = 0; i < vec_length; i++)
		{
			if ((int)bits[i] == 0)
			{
				llr_0_noninfinite_count++;
				llr_0_min = std::min((double)llrs[i], llr_0_min);
				llr_0_max = std::max((double)llrs[i], llr_0_max);
			}
			else
			{
				llr_1_noninfinite_count++;
				llr_1_min = std::min((double)llrs[i], llr_1_min);
				llr_1_max = std::max((double)llrs[i], llr_1_max);
			}
		}
		if (llr_0_noninfinite_count > 0 && llr_1_noninfinite_count > 0 && 
		    llr_0_min <= llr_1_max && llr_1_min <= llr_0_max)
		{
			for (int i = 0; i < vec_length; i++)
			{
				if ((double)llrs[i] != -inf && (double)llrs[i] != inf)
				{
					if ((int)bits[i] == 0)
						llr_0_mean = llr_0_mean + (double)llrs[i];
					else
						llr_1_mean = llr_1_mean + (double)llrs[i];
				}
			}
			llr_0_mean = llr_0_mean / llr_0_noninfinite_count;
			llr_1_mean = llr_1_mean / llr_1_noninfinite_count;

			for (int i = 0; i < vec_length; i++)
			{
				if ((double)llrs[i] != -inf && (double)llrs[i] != inf)
				{
					if ((int)bits[i] == 0)
						llr_0_variance = llr_0_variance + std::pow(((double)llrs[i] - llr_0_mean), 2);
					else
						llr_1_variance = llr_1_variance + std::pow(((double)llrs[i] - llr_1_mean), 2);
				}
			}
			llr_0_variance = llr_0_variance / llr_0_noninfinite_count;
			llr_1_variance = llr_1_variance / llr_1_noninfinite_count;

			bin_width = 0.5 * (3.49 * std::sqrt(llr_0_variance) * (std::pow(llr_0_noninfinite_count, (-1.0 / 3.0))) + 
			            3.49 * std::sqrt(llr_1_variance) * (std::pow(llr_1_noninfinite_count, (-1.0 / 3.0))));
			if (bin_width > 0.0)
			{
				bin_offset = (int)std::floor(std::min(llr_0_min, llr_1_min) / bin_width) - 1;
				tmp = std::max(llr_0_max, llr_1_max) / bin_width - bin_offset + 1;
				bin_count = (int)std::ceil(tmp);
				if (bin_count == tmp)
					bin_count = bin_count + 1;
			}
			else
			{
				bin_offset = -1;
				bin_count  = 3;
			}
			lots_of_bins = true;
		}
		else
		{
			lots_of_bins = false;
			bin_count    = 4;
		}

		std::vector<std::vector<double> > histogram(2, std::vector<double>(bin_count));
		std::vector<std::vector<double> > pdf(2, std::vector<double>(bin_count));
		for (int i = 0; i < vec_length; i++)
		{
			if ((double)llrs[i] == -inf)
				histogram[(int)bits[i]][0] += 1;
			else if ((double)llrs[i] == inf)
				histogram[(int)bits[i]][bin_count - 1] += 1;
			else
			{
				if (lots_of_bins)
				{
					if (bin_width > 0.0)
						histogram[(int)bits[i]][(int)(std::floor((double)llrs[i] / bin_width) - bin_offset)] += 1.0;
					else
						histogram[(int)bits[i]][1] += 1;
				}
				else
					histogram[(int)bits[i]][ (int)bits[i] + 1] += 1;
			}
		}

		for (int i = 0; i < bin_count; i++)
		{
			pdf[0][i] = histogram[0][i] / bit_0_count;
			pdf[1][i] = histogram[1][i] / bit_1_count;
		}

		I_E = 0.0;
		for (int b = 0; b < 2; b++)
			for (int bin_ix = 0; bin_ix < bin_count; bin_ix++)
				if (pdf[b][bin_ix] > 0.0)
					I_E += 0.5 * pdf[b][bin_ix] * std::log2(2.0 * pdf[b][bin_ix] / (pdf[0][bin_ix] + pdf[1][bin_ix]));
	}

	return I_E;
}

// ---------------------------------------------------------------------------------------------------- virtual methods

template <typename B, typename R>
void EXIT<B,R>
::release_objects()
{
	if (source    != nullptr) { delete source;    source    = nullptr; }
	if (encoder   != nullptr) { delete encoder;   encoder   = nullptr; }
	if (modem     != nullptr) { delete modem;     modem     = nullptr; }
	if (modem_a   != nullptr) { delete modem_a;   modem_a   = nullptr; }
	if (channel   != nullptr) { delete channel;   channel   = nullptr; }
	if (channel_a != nullptr) { delete channel_a; channel_a = nullptr; }
	if (siso      != nullptr) { delete siso;      siso      = nullptr; }
	if (terminal  != nullptr) { delete terminal;  terminal  = nullptr; }
}

template <typename B, typename R>
Source<B>* EXIT<B,R>
::build_source()
{
	return factory::Source::build<B>(*params.src);
}

template <typename B, typename R>
Encoder<B>* EXIT<B,R>
::build_encoder()
{
	return this->codec.build_encoder();
}

template <typename B, typename R>
Modem<B,R,R>* EXIT<B,R>
::build_modem()
{
	auto mdm_cpy = *params.mdm;
	if (params.mdm->sigma == -1.f)
		mdm_cpy.sigma = this->sigma;
	return factory::Modem::build<B,R>(mdm_cpy);
}

template <typename B, typename R>
Modem<B,R,R>* EXIT<B,R>
::build_modem_a()
{
	auto mdm_cpy  = *params.mdm;
	mdm_cpy.sigma = 2.f / sig_a;
	mdm_cpy.N     = params.enc->K;
	return factory::Modem::build<B,R>(mdm_cpy);
}

template <typename B, typename R>
Channel<R>* EXIT<B,R>
::build_channel(const int size)
{
	auto chn_cpy = *params.chn;
	if (params.chn->sigma == -1.f)
		chn_cpy.sigma = this->sigma;
	return factory::Channel::build<R>(chn_cpy);
}

template <typename B, typename R>
Channel<R>* EXIT<B,R>
::build_channel_a(const int size)
{
	auto chn_cpy  = *params.chn;
	chn_cpy.sigma = 2.f / sig_a;
	chn_cpy.N     = factory::Modem::get_buffer_size_after_modulation(params.mdm->type,
	                                                                 params.enc->K,
	                                                                 params.mdm->bps,
	                                                                 params.mdm->upf,
 	                                                                 params.mdm->cpm_L);
	return factory::Channel::build<R>(chn_cpy);
}

template <typename B, typename R>
Decoder_SISO<R>* EXIT<B,R>
::build_siso()
{
	return this->codec.build_siso();
}

// ------------------------------------------------------------------------------------------------- non-virtual method

template <typename B, typename R>
Terminal_EXIT* EXIT<B,R>
::build_terminal()
{
	auto term_cpy = *params.ter;
	term_cpy.snr   = snr;
	term_cpy.sig_a = sig_a;
	return factory::Terminal_EXIT::build(term_cpy, cur_trial, n_trials, I_A, I_E);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::EXIT<B_8,R_8>;
template class aff3ct::simulation::EXIT<B_16,R_16>;
template class aff3ct::simulation::EXIT<B_32,R_32>;
template class aff3ct::simulation::EXIT<B_64,R_64>;
#else
template class aff3ct::simulation::EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation
