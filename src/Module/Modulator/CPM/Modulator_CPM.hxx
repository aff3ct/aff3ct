#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctgmath>

#include "Tools/Math/matrix.h"
#include "Modulator_CPM.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_CPM<B,R,Q,MAX>
::Modulator_CPM(int  N,
                int  bits_per_symbol,
                int  sampling_factor,
                std::string cpm_std,
                int  cpm_L,
                int  cpm_k,
                int  cpm_p,
                std::string mapping,
                std::string wave_shape,
                R    sigma,
                bool no_sig2,
                int  n_frames,
                const std::string name)
: Modulator<B,R,Q>(N, 0, 0, n_frames, name      ), // TODO: warning: won't work if N_mod and N_fil are used since this
                                                   // constructor is not completely done
  N         (N                                  ),
  sigma     (sigma                              ),
  no_sig2   (no_sig2                            ),
  cpm       (cpm_std,
             cpm_L,
             cpm_k,
             cpm_p,
             bits_per_symbol,
             sampling_factor,
             "TOTAL",
             wave_shape                         ),
  cpm_h     ((R)cpm.k/(R)cpm.p                  ),
  T_samp    ((R)1.0  /(R)cpm.s_factor           ),
  baseband  (  cpm.max_wa_id *cpm.s_factor *2, 0),
  projection(cpm.max_wa_id *cpm.s_factor *2,   0),
  n_sy      (N/cpm.n_b_per_s                    ),
  n_sy_tl   (n_sy+cpm.tl                        ),
  cpe       (n_sy, cpm, this->n_frames          ),
  bcjr      (cpm, n_sy_tl                       )
{
	assert(N%bits_per_symbol == 0);

	// write the right buffers sizes
	int* N_mod_writable = const_cast<int*>(&this->N_mod     );
	*N_mod_writable     = get_buffer_size_after_modulation(N);
	int* N_fil_writable = const_cast<int*>(&this->N_fil     );
	*N_fil_writable     = get_buffer_size_after_filtering(N );

	// initialize CPM
	cpe.generate_allowed_states    (cpm.allowed_states               );
	cpe.generate_allowed_wave_forms(cpm.allowed_wave_forms           );

	cpe.generate_mapper            (cpm.transition_to_binary,
	                                cpm.binary_to_transition,
	                                mapping                          );

	cpe.generate_trellis           (cpm.trellis_next_state,
	                                cpm.trellis_related_wave_form    );
	cpe.generate_anti_trellis      (cpm.anti_trellis_original_state,
	                                cpm.anti_trellis_input_transition);

	generate_baseband              (                                 );
	generate_projection            (                                 );
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_CPM<B,R,Q,MAX>
::~Modulator_CPM()
{
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_CPM<B,R,Q,MAX>
::get_buffer_size_after_modulation(const int N)
{
	// N : number of bits to send
	//  / cpm.n_b_per_s: because buffer contains symbols and not bits anymore
	// +cpm.tl: tails symbols
	// *cpm.s_factor: because work with samples of wave forms
	// *2: because of complex numbers
	return (N / cpm.n_b_per_s + cpm.tl) * cpm.s_factor * 2;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_CPM<B,R,Q,MAX>
::get_buffer_size_after_filtering(const int N)
{
	// N : number of bits to receive
	// / cpm.n_b_per_s: because buffer contains symbols and not bits
	// +cpm.tl: tails symbols
	// *cpm.n_wa: because work with waveforms probability
	return (N / cpm.n_b_per_s + cpm.tl) * cpm.max_wa_id;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	assert((int)X_N1.size() ==           N*this->n_frames);
	assert((int)X_N2.size() == this->N_mod*this->n_frames);

	if (this->n_frames == 1)
		_modulate(X_N1, X_N2);
	else // more than 1 frame
	{
		mipp::vector<B> X_N1_tmp(N);
		mipp::vector<R> X_N2_tmp(this->N_mod);
		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(X_N1.begin() +  f     * N,
			          X_N1.begin() + (f +1) * N,
			          X_N1_tmp.begin());

			_modulate(X_N1_tmp, X_N2_tmp);

			std::copy(X_N2_tmp.begin(), X_N2_tmp.end(), X_N2.begin() + f * this->N_mod);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::_modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	assert((int)X_N1.size() == N          );
	assert((int)X_N2.size() == this->N_mod);

	// mapper
	mipp::vector<SIN> mapped_frame(n_sy);

	for (int i=0; i < n_sy; i++)
		mapped_frame[i] = cpm.binary_to_transition[cpe.merge_bits(X_N1.data()+i*cpm.n_b_per_s, cpm.n_b_per_s, true)];

	// continuous phase encoder
	mipp::vector<SIN> encoded_frame(n_sy_tl);
	cpe.encode(mapped_frame, encoded_frame);

	// memoryless modulation (attributing complex waveforms to symbols)
	const auto off_X_r  = 0;
	const auto off_X_i  = (int)X_N2.size() / 2;
	const auto off_BB_r = 0;
	const auto off_BB_i = (int)baseband.size() / 2;

	for (auto i = 0; i < n_sy_tl; i++)
		for (auto s = 0; s < cpm.s_factor; s++)
		{
			X_N2[off_X_r + i * cpm.s_factor + s] = baseband[off_BB_r + encoded_frame[i] * cpm.s_factor + s];
			X_N2[off_X_i + i * cpm.s_factor + s] = baseband[off_BB_i + encoded_frame[i] * cpm.s_factor + s];
		}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	assert((int)Y_N1.size() == this->N_mod * this->n_frames);
	assert((int)Y_N2.size() == this->N_fil * this->n_frames);

	if (this->n_frames == 1)
		_filter(Y_N1, Y_N2);
	else // more than 1 frame
	{
		mipp::vector<R> Y_N1_tmp(this->N_mod);
		mipp::vector<R> Y_N2_tmp(this->N_fil);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N_mod,
			          Y_N1.begin() + (f +1) * this->N_mod,
			          Y_N1_tmp.begin());

			_filter(Y_N1_tmp, Y_N2_tmp);

			std::copy(Y_N2_tmp.begin(), Y_N2_tmp.end(), Y_N2.begin() + f * this->N_fil);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::_filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	assert((int)Y_N1.size() == this->N_mod);
	assert((int)Y_N2.size() == this->N_fil);

	const auto Y_real = Y_N1.data();
	const auto Y_imag = Y_N1.data() + (Y_N1.size() >> 1);
	const auto p_real = projection.data();
	const auto p_imag = projection.data() + (projection.size() >> 1);

	for (auto i = 0; i < n_sy_tl; i++)
		for (auto wa = 0; wa < cpm.n_wa; wa++)
		{
			R sum_r = (R)0;
			for (auto s = 0; s < cpm.s_factor; s++)
				sum_r += Y_real[i * cpm.s_factor + s] * p_real[s * cpm.max_wa_id + cpm.allowed_wave_forms[wa]]
				       - Y_imag[i * cpm.s_factor + s] * p_imag[s * cpm.max_wa_id + cpm.allowed_wave_forms[wa]];

			Y_N2[i * cpm.max_wa_id + cpm.allowed_wave_forms[wa]] = sum_r;
		}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert((int)Y_N1.size() == this->N_fil * this->n_frames);
	assert((int)Y_N2.size() ==           N * this->n_frames);

	if (this->n_frames == 1)
		bcjr.decode(Y_N1, Y_N2);
	else // more than 1 frame
	{
		mipp::vector<Q> Y_N1_tmp(this->N_fil);
		mipp::vector<Q> Y_N2_tmp(N);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N_fil,
			          Y_N1.begin() + (f +1) * this->N_fil,
			          Y_N1_tmp.begin());

			bcjr.decode(Y_N1_tmp, Y_N2_tmp);

			std::copy(Y_N2_tmp.begin(), Y_N2_tmp.end(), Y_N2.begin() + f * N);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert((int)Y_N1.size() == this->n_frames * this->N_fil);
	assert((int)Y_N2.size() == this->n_frames * N          );
	assert((int)Y_N3.size() == this->n_frames * N          );

	if (this->n_frames == 1)
		bcjr.decode(Y_N1, Y_N2, Y_N3);
	else
	{
		mipp::vector<Q> Y_N1_tmp(this->N_fil);
		mipp::vector<Q> Y_N2_tmp(N);
		mipp::vector<Q> Y_N3_tmp(N);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N_fil,
			          Y_N1.begin() + (f +1) * this->N_fil,
			          Y_N1_tmp.begin());

			std::copy(Y_N2.begin() + (f +0) * N,
			          Y_N2.begin() + (f +1) * N,
			          Y_N2_tmp.begin());

			bcjr.decode(Y_N1_tmp, Y_N2_tmp, Y_N3_tmp); // remove tail symb automatically because
			                                           // Y_N3_tmp.size = Y_N1_tmp.size + cpm.tl

			std::copy(Y_N3_tmp.begin(), Y_N3_tmp.end(), Y_N3.begin() + f * N);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::generate_baseband()
{
	assert((int)baseband.size() == (cpm.max_wa_id*cpm.s_factor*2));
	mipp::vector<R> phase_response(cpm.L*cpm.s_factor);

	// calculate the different phase responses
	for (auto s = 0; s < cpm.L * cpm.s_factor; s++)
		phase_response[s] = calculate_phase_response(s * T_samp);

	auto p_mask = (1 << cpm.n_bits_p ) -1;
	auto L_mask = (1 << cpm.n_b_per_s) -1;

	for (auto wa = 0; wa < cpm.n_wa; wa++)
	{
		auto allowed_wa         = cpm.allowed_wave_forms[wa];
		auto tilted_phase_part1 = (R)(2 * M_PI * cpm_h * (allowed_wa & p_mask));

		mipp::vector<R> tilted_phase_part2(cpm.s_factor, (R)0);
		mipp::vector<R> tilted_phase_part3(cpm.s_factor, (R)0);

		for (auto l = 0; l < cpm.L; l++)
		{
			auto U_n = (allowed_wa >> ((cpm.L -l -1) * cpm.n_b_per_s + cpm.n_bits_p)) & L_mask;

			for (auto s = 0; s < cpm.s_factor; s++)
			{
				tilted_phase_part2[s] += phase_response[l * cpm.s_factor +s] * U_n;
				tilted_phase_part3[s] += phase_response[l * cpm.s_factor +s];
			}
		}

		for (auto s = 0; s < cpm.s_factor; s++)
		{
			R tilted_phase = tilted_phase_part1 + M_PI * cpm_h * (4 * tilted_phase_part2[s] +
			                 (cpm.m_order -1) * (s * T_samp + (cpm.L -1) - 2 * tilted_phase_part3[s]));

			baseband[allowed_wa * cpm.s_factor + s                      ] = std::cos(tilted_phase);
			baseband[allowed_wa * cpm.s_factor + s + baseband.size() / 2] = std::sin(tilted_phase);
		}
	}
}

template <typename R>
class GMSK
{
private:
	const R B;
	const R off;

public:
	GMSK(const R b, const R offset) : B(b), off(offset){ }

	R operator()(const R& t) const
	{
		R x      = t + off;
		R factor = (R)2.0 * M_PI * B / sqrt((R)2.0 * std::log((R)2.0));
		R minus  = (x - (R)0.5) * factor;
		R plus   = (x + (R)0.5) * factor;

		return (std::erf(plus) - std::erf(minus))/(R)4.0;
	}
};

template <typename B, typename R, typename Q, proto_max<Q> MAX>
R Modulator_CPM<B,R,Q,MAX>
::calculate_phase_response(const R t_stamp)
{
	if (cpm.wave_shape == "GMSK")
	{
		if (t_stamp == (R)0.0)
			return (R)0.0;

		GMSK<R> g((R)0.3, -(R)cpm.L / (R)2.0);
		return integral(g, (R)0.0, t_stamp, t_stamp / (R)1e-4);
	}
	else if (cpm.wave_shape == "RCOS")
		return t_stamp / ((R)2.0 * cpm.L) - sin((R)2.0 * M_PI * t_stamp / (R)cpm.L) / (R)4.0 / M_PI;
	else if (cpm.wave_shape == "REC")
		return t_stamp / ((R)2.0 * cpm.L);
	else
	{
		std::cerr << bold_red("(EE) Unknown CPM wave shape") << std::endl;
		exit(-1);
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::generate_projection()
{
	assert(projection.size() == baseband.size());

	R factor = (R)1;

	if (!no_sig2)
		factor = (R)1 / (sigma * sigma);

	if (cpm.filters_type == "TOTAL")
	{
		complex_transpose(cpm.max_wa_id, cpm.s_factor, baseband, projection);

		for (auto i = 0; i < (int)projection.size() ; i++)
			projection[i] *= factor;
	}
	//else if(filters_type == "ORTHO_NORM")
	//{
	//}
	else
	{
		std::cerr << bold_red("(EE) Unknown CPM filter bank type") << std::endl;
		exit(-1);
	}
}
