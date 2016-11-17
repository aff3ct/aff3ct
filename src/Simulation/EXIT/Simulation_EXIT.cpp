#include <algorithm>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_modulator.hpp"
#include "Tools/Factory/Factory_channel.hpp"

#include "Simulation_EXIT.hpp"

template <typename B, typename R, typename Q>
Simulation_EXIT<B,R,Q>
::Simulation_EXIT(const parameters& params)
: Simulation(),

  params(params),
  H_N        (params.code.N + params.code.tail_length                      ),
  B_K        (params.code.K                                                ),
  B_N        (params.code.N + params.code.tail_length                      ),
  X_K1       (params.code.K                                                ),
  X_N1       (params.code.N + params.code.tail_length                      ),
  X_K2       (params.code.K                                                ),
  X_N2       (params.code.N + params.code.tail_length                      ),
  Y_N        (params.code.N + params.code.tail_length                      ),
  Y_K        (params.code.K                                                ),
  La_K1      (params.code.K                                                ),
  Lch_N1     (params.code.N + params.code.tail_length                      ),
  La_K2      (params.code.K                                                ),
  Lch_N2     (params.code.N + params.code.tail_length                      ),
  Le_K       (params.code.K                                                ),
  sys        (params.code.K                                                ),
  par        ((params.code.N - params.code.K) + (params.code.tail_length/2)),

  B_buff (0),
  Le_buff(0),
  La_buff(0),

  n_trials (200000 / params.code.K),
  cur_trial(0),

  I_A      (0.0),
  I_E      (0.0),
  sig_a    (0.f),
  code_rate(0.f),
  sigma    (0.f),
  snr      (0.f),

  source     (nullptr),
  encoder    (nullptr),
  modulator  (nullptr),
  modulator_a(nullptr),
  channel    (nullptr),
  channel_a  (nullptr),
  siso       (nullptr),
  terminal   (nullptr)
{
	if (typeid(R) != typeid(Q))
		std::cerr << bold_yellow("(WW) EXIT simulation does not use fixed-point representation!") << std::endl;

#ifdef ENABLE_MPI
	std::clog << bold_yellow("(WW) This simulation is not MPI ready, the same computations will be launched ")
	          << bold_yellow("on each MPI processes.") << std::endl;
#endif
}

template <typename B, typename R, typename Q>
Simulation_EXIT<B,R,Q>
::~Simulation_EXIT()
{
	release_objects();
}

template <typename B, typename R, typename Q>
void Simulation_EXIT<B,R,Q>
::build_communication_chain()
{
	release_objects();

	// build the objects
	source      = build_source     (      ); check_errors(source     , "Source<B>"         );
	encoder     = build_encoder    (      ); check_errors(encoder    , "Encoder<B>"        );
	modulator   = build_modulator  (      ); check_errors(modulator  , "Modulator<B,R>"    );
	modulator_a = build_modulator_a(      ); check_errors(modulator_a, "Modulator<B,R>"    );

	const auto N     = params.code.N;
	const auto tail  = params.code.tail_length;
	const auto N_mod = modulator->get_buffer_size_after_modulation(N + tail);

	channel     = build_channel    (N_mod ); check_errors(channel    , "Channel<R>"        );
	channel_a   = build_channel_a  (N_mod ); check_errors(channel    , "Channel<R>"        );
	siso        = build_siso       (      ); check_errors(siso       , "SISO<R>"           );
	terminal    = build_terminal   (      ); check_errors(terminal   , "Terminal_EXIT<B,R>");

	if (siso->get_n_frames() > 1)
	{
		std::cout << bold_red("(EE) EXIT simulation does not support inter frame SIMD... Exiting.") << std::endl;
		exit(-1);
	}

	// resize the modulation buffers
	const auto K_mod = modulator_a->get_buffer_size_after_modulation(params.code.K);
	if (X_K2  .size() != (unsigned)  K_mod        ) X_K2  .resize(K_mod       );
	if (X_N2  .size() != (unsigned) (N_mod + tail)) X_N2  .resize(N_mod + tail);
	if (La_K1 .size() != (unsigned)  K_mod        ) La_K1 .resize(K_mod       );
	if (Lch_N1.size() != (unsigned) (N_mod + tail)) Lch_N1.resize(N_mod + tail);
	if (H_N   .size() != (unsigned) (N_mod + tail)) H_N   .resize(N_mod + tail);
}

template <typename B, typename R, typename Q>
void Simulation_EXIT<B,R,Q>
::launch()
{
	bool first_loop = true;

	launch_precompute();

	// for each channel SNR to be simulated	
	for (snr = params.simulation.snr_min; snr <= params.simulation.snr_max; snr += params.simulation.snr_step)
	{
		// For EXIT simulation, SNR is considered as Es/N0
		code_rate = 1.f;
		sigma     = std::sqrt((float)params.modulator.upsample_factor) /
		            std::sqrt(2.f * code_rate * (float)params.modulator.bits_per_symbol * std::pow(10.f, (snr / 10.f)));

		snr_precompute();

		// for each "a" standard deviation (sig_a) to be simulated
		for (sig_a = params.simulation.sig_a_min; sig_a <= params.simulation.sig_a_max; sig_a += params.simulation.sig_a_step)
		{
			I_A = 0.0;
			I_E = 0.0;

			t_snr = std::chrono::steady_clock::now();

			// allocate and build all the communication chain to generate EXIT chart
			this->build_communication_chain();

			// if sig_a = 0, La_K2 = 0
			if (sig_a == 0)
				std::fill(La_K2.begin(), La_K2.end(), params.channel.domain == "LLR" ? init_LLR<R>() : init_LR<R>());

			if (!params.terminal.disabled && first_loop && !params.simulation.debug)
			{
				terminal->legend(std::cout);
				first_loop = false;
			}

			this->simulation_loop();

			if (!params.terminal.disabled)
				terminal->final_report(std::cout);
		}
	}
}

template <typename B, typename R, typename Q>
void Simulation_EXIT<B,R,Q>
::simulation_loop()
{
	using namespace std::chrono;

	// simulation loop
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

		// X_K used to generate La_K vector
		X_K1 = B_K;

		// modulate
		modulator_a->modulate(X_K1, X_K2);
		modulator  ->modulate(X_N1, X_N2);

		//if sig_a = 0, La_K = 0, no noise to add
		if (sig_a != 0)
		{
			// Rayleigh channel
			if (params.channel.type.find("RAYLEIGH") != std::string::npos)
			{
				channel_a->add_noise(X_K2, La_K1, H_N);
				modulator_a->demodulate_with_gains(La_K1, H_N, La_K2);
			}
			else // additive channel (AWGN, USER, NO)
			{
				channel_a  ->add_noise (X_K2,  La_K1);
				modulator_a->demodulate(La_K1, La_K2);
			}
		}

		// Rayleigh channel
		if (params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			channel->add_noise(X_N2, Lch_N1, H_N);
			modulator->demodulate_with_gains(Lch_N1, H_N, Lch_N2);
		}
		else // additive channel (AWGN, USER, NO)
		{
			channel  ->add_noise (X_N2,   Lch_N1);
			modulator->demodulate(Lch_N1, Lch_N2);
		}

		// extract systematic and parity information
		extract_sys_par(Lch_N2, La_K2, sys, par);

		// decode
		siso->soft_decode(sys, par, Le_K);

		// store B_K, La_K and Le_K in buffers (add current B and L to the buffers)
		B_buff .insert(B_buff .end(), B_K  .begin(), B_K  .end());
		Le_buff.insert(Le_buff.end(), Le_K .begin(), Le_K .end());
		La_buff.insert(La_buff.end(), La_K2.begin(), La_K2.end());

		// display statistics in terminal
		if (!params.terminal.disabled && (steady_clock::now() - t_simu) >= params.terminal.frequency)
		{
			terminal->temp_report(std::clog);
			t_simu = steady_clock::now();
		}
	}

	// measure mutual information and store it in I_A, I_E, sig_a_array
	I_A = measure_mutual_info_avg  (La_buff, B_buff) / (params.code.K * n_trials);
	I_E = measure_mutual_info_histo(Le_buff, B_buff, 1000);
}

template <typename B, typename R, typename Q>
double Simulation_EXIT<B,R,Q>
::measure_mutual_info_avg(const mipp::vector<R>& llrs, const mipp::vector<B>& bits)
{
	double I_A = 0;
	double symb;
	auto size = (int)llrs.size();
	for (int j = 0; j < size; j++)
	{
		symb = -2 * (double)bits[j] +1;
		I_A += (1 - log2(1 + std::exp(-symb * (double)llrs[j])));
	}

	return(I_A);
}

template <typename B, typename R, typename Q>
double Simulation_EXIT<B,R,Q>
::measure_mutual_info_histo(const mipp::vector<R>& llrs, const mipp::vector<B>& bits, const int N)
{
	const double Inf = 10000000;

	int vec_length = (int)bits.size();

	bool lots_of_bins;
	int bin_count, bin_offset = 0, bit_0_count, bit_1_count;
	int llr_0_noninfinite_count = 0;
	int llr_1_noninfinite_count = 0;

	double tmp;
	double I_E = 0.0;
	double bin_width = 0.0f;
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
		llr_0_max = llr_1_max = -Inf;
		llr_0_min = llr_1_min = +Inf;

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
				if ((double)llrs[i] != -Inf && (double)llrs[i] != Inf)
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
				if ((double)llrs[i] != -Inf && (double)llrs[i] != Inf)
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
			if ((double)llrs[i] == -Inf)
				histogram[(int)bits[i]][0] += 1;
			else if ((double)llrs[i] == Inf)
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

template <typename B, typename R, typename Q>
void Simulation_EXIT<B,R,Q>
::release_objects()
{
	if (source      != nullptr) { delete source;      source      = nullptr; }
	if (encoder     != nullptr) { delete encoder;     encoder     = nullptr; }
	if (modulator   != nullptr) { delete modulator;   modulator   = nullptr; }
	if (modulator_a != nullptr) { delete modulator_a; modulator_a = nullptr; }
	if (channel     != nullptr) { delete channel;     channel     = nullptr; }
	if (channel_a   != nullptr) { delete channel_a;   channel_a   = nullptr; }
	if (siso        != nullptr) { delete siso;        siso        = nullptr; }
	if (terminal    != nullptr) { delete terminal;    terminal    = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_EXIT<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_EXIT<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_EXIT<B,R,Q>
::build_source()
{
	return Factory_source<B>::build(params);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_EXIT<B,R,Q>
::build_encoder()
{
	return Factory_encoder_common<B>::build(params, params.simulation.seed);
}

template <typename B, typename R, typename Q>
Modulator<B,R,R>* Simulation_EXIT<B,R,Q>
::build_modulator()
{
	return Factory_modulator<B,R,R>::build(params, sigma);
}

template <typename B, typename R, typename Q>
Modulator<B,R,R>* Simulation_EXIT<B,R,Q>
::build_modulator_a()
{
	return Factory_modulator<B,R,R>::build(params, 2.f / sig_a);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_EXIT<B,R,Q>
::build_channel(const int size)
{
	return Factory_channel<R>::build(params, sigma, size, params.simulation.seed);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_EXIT<B,R,Q>
::build_channel_a(const int size)
{
	return Factory_channel<R>::build(params, 2.f / sig_a, size, params.simulation.seed);
}

// ------------------------------------------------------------------------------------------------- non-virtual method

template <typename B, typename R, typename Q>
Terminal_EXIT<B,R>* Simulation_EXIT<B,R,Q>
::build_terminal()
{
	return new Terminal_EXIT<B,R>(params.code.N, snr, sig_a, t_snr, cur_trial, n_trials, I_A, I_E);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_EXIT<B_8,R_8,Q_8>;
template class Simulation_EXIT<B_16,R_16,Q_16>;
template class Simulation_EXIT<B_32,R_32,Q_32>;
template class Simulation_EXIT<B_64,R_64,Q_64>;
#else
template class Simulation_EXIT<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
