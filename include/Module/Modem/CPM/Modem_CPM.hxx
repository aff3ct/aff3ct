#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <string>
#include <cmath>
#include <vector>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/matrix.h"
#include "Tools/Math/numerical_integration.h"
#include "Tools/Noise/Noise.hpp"
#include "Module/Modem/CPM/Modem_CPM.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
const std::string Modem_CPM<B,R,Q,MAX>::mapping_default = "NATURAL";
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
const std::string Modem_CPM<B,R,Q,MAX>::wave_shape_default = "GMSK";

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modem_CPM<B,R,Q,MAX>
::Modem_CPM(const int  N,
            const int  bits_per_symbol,
            const int  sampling_factor,
            const int  cpm_L,
            const int  cpm_k,
            const int  cpm_p,
            const std::string &mapping,
            const std::string &wave_shape,
            const bool no_sig2)
: Modem<B,R,Q>(N,
               Modem_CPM<B,R,Q,MAX>::size_mod(N, bits_per_symbol, cpm_L, cpm_p, sampling_factor),
               Modem_CPM<B,R,Q,MAX>::size_fil(N, bits_per_symbol, cpm_L, cpm_p)),
  no_sig2   (no_sig2                             ),
  cpm       (new tools::CPM_parameters<SIN,SOUT>(cpm_L,
                                                 cpm_k,
                                                 cpm_p,
                                                 bits_per_symbol,
                                                 sampling_factor,
                                                 "TOTAL",
                                                 wave_shape)),
  cpm_h     ((R)cpm->k/(R)cpm->p                 ),
  T_samp    ((R)1.0  /(R)cpm->s_factor           ),
  baseband  (cpm->max_wa_id * cpm->s_factor *2, 0),
  projection(cpm->max_wa_id * cpm->s_factor *2, 0),
  n_sy      (N/cpm->n_b_per_s                    ),
  n_sy_tl   (n_sy+cpm->tl                        ),
  cpe       (n_sy, *cpm                          ),
  bcjr      (*cpm, n_sy_tl                       )
{
	const std::string name = "Modem_CPM";
	this->set_name(name);

	this->set_filter(true);

	if (N % bits_per_symbol)
	{
		std::stringstream message;
		message << "'bits_per_symbol' has to be a multiple of 'N' ('bits_per_symbol' = " << bits_per_symbol
		        << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// initialize CPM
	cpe.generate_allowed_states    (cpm->allowed_states               );
	cpe.generate_allowed_wave_forms(cpm->allowed_wave_forms           );

	cpe.generate_mapper            (cpm->transition_to_binary,
	                                cpm->binary_to_transition,
	                                mapping                           );

	cpe.generate_trellis           (cpm->trellis_next_state,
	                                cpm->trellis_related_wave_form    );
	cpe.generate_anti_trellis      (cpm->anti_trellis_original_state,
	                                cpm->anti_trellis_input_transition);

	cpe.generate_tail_symb_transition();

	generate_baseband();

	if (no_sig2)
		this->generate_projection((R)1);
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modem_CPM<B,R,Q,MAX>* Modem_CPM<B,R,Q,MAX>
::clone() const
{
	auto m = new Modem_CPM(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
bool Modem_CPM<B,R,Q,MAX>
::is_complex_mod()
{
	return true;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
bool Modem_CPM<B,R,Q,MAX>
::is_complex_fil()
{
	return false;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
int Modem_CPM<B,R,Q,MAX>
::size_mod(const int N, const int bps, const int L, const int p, const int ups)
{
	int m_order = (int)1 << bps;
	int n_tl	= (int)(std::ceil((float)(p - 1) / (float)(m_order - 1))) + L - 1;

	return Modem<B,R,Q>::get_buffer_size_after_modulation(N, bps, n_tl, ups, is_complex_mod());
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
int Modem_CPM<B,R,Q,MAX>
::size_fil(const int N, const int bps, const int L, const int p)
{
	int m_order   = (int)1 << bps;
	int n_tl	  = (int)(std::ceil((float)(p - 1) / (float)(m_order - 1))) + L - 1;
	int n_wa      = (int)(p * std::pow(m_order, L));
	int n_bits_wa = (int)std::ceil(std::log2(n_wa));
	int max_wa_id = (int)(1 << n_bits_wa);

	return Modem<B,R,Q>::get_buffer_size_after_filtering(N, bps, n_tl, max_wa_id, is_complex_fil());
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_CPM<B,R,Q,MAX>
::_modulate(const B *X_N1, R *X_N2, const size_t frame_id)
{
	// mapper
	std::vector<SIN> mapped_frame(n_sy);

	for (int i=0; i < n_sy; i++)
		mapped_frame[i] = cpm->binary_to_transition[cpe.merge_bits(X_N1+i*cpm->n_b_per_s, cpm->n_b_per_s, true)];

	// continuous phase encoder
	std::vector<SIN> encoded_frame(n_sy_tl);
	cpe.encode(mapped_frame, encoded_frame);

	// memoryless modulation (attributing complex waveforms to symbols)
	const auto off_X_r  = 0;
	const auto off_X_i  = this->N_mod / 2;
	const auto off_BB_r = 0;
	const auto off_BB_i = (int)baseband.size() / 2;

	for (auto i = 0; i < n_sy_tl; i++)
		for (auto s = 0; s < cpm->s_factor; s++)
		{
			X_N2[off_X_r + i * cpm->s_factor + s] = baseband[off_BB_r + encoded_frame[i] * cpm->s_factor + s];
			X_N2[off_X_i + i * cpm->s_factor + s] = baseband[off_BB_i + encoded_frame[i] * cpm->s_factor + s];
		}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_CPM<B,R,Q,MAX>
::_filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	if (*CP != this->last_channel_param && !no_sig2)
		this->generate_projection((R)1 / ((R)*CP * (R)*CP));

	const auto Y_real = Y_N1;
	const auto Y_imag = Y_N1 + this->N_mod / 2;
	const auto p_real = projection.data();
	const auto p_imag = projection.data() + (projection.size() >> 1);

	for (auto i = 0; i < n_sy_tl; i++)
		for (auto wa = 0; wa < cpm->n_wa; wa++)
		{
			R sum_r = (R)0;
			for (auto s = 0; s < cpm->s_factor; s++)
				sum_r += Y_real[i * cpm->s_factor + s] * p_real[s * cpm->max_wa_id + cpm->allowed_wave_forms[wa]]
				       - Y_imag[i * cpm->s_factor + s] * p_imag[s * cpm->max_wa_id + cpm->allowed_wave_forms[wa]];

			Y_N2[i * cpm->max_wa_id + cpm->allowed_wave_forms[wa]] = sum_r;
		}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_CPM<B,R,Q,MAX>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	bcjr.decode(Y_N1, Y_N2);
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_CPM<B,R,Q,MAX>
::_tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	bcjr.decode(Y_N1, Y_N2, Y_N3);
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_CPM<B,R,Q,MAX>
::generate_baseband()
{
	if ((int)baseband.size() != (cpm->max_wa_id * cpm->s_factor * 2))
	{
		std::stringstream message;
		message << "'baseband.size()' has to be equal to 'cpm->max_wa_id' * 'cpm->s_factor' * 2 ('baseband.size()' = "
		        << baseband.size() << ", 'cpm->max_wa_id' = " << cpm->max_wa_id
		        << ", 'cpm->s_factor' = " << cpm->s_factor << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<R> phase_response(cpm->L*cpm->s_factor);

	// calculate the different phase responses
	for (auto s = 0; s < cpm->L * cpm->s_factor; s++)
		phase_response[s] = calculate_phase_response(s * T_samp);

	auto p_mask = (1 << cpm->n_bits_p ) -1;
	auto L_mask = (1 << cpm->n_b_per_s) -1;

	for (auto wa = 0; wa < cpm->n_wa; wa++)
	{
		auto allowed_wa         = cpm->allowed_wave_forms[wa];
		auto tilted_phase_part1 = (R)(2 * (R)M_PI * cpm_h * (allowed_wa & p_mask));

		std::vector<R> tilted_phase_part2(cpm->s_factor, (R)0);
		std::vector<R> tilted_phase_part3(cpm->s_factor, (R)0);

		for (auto l = 0; l < cpm->L; l++)
		{
			auto U_n = (allowed_wa >> ((cpm->L -l -1) * cpm->n_b_per_s + cpm->n_bits_p)) & L_mask;

			for (auto s = 0; s < cpm->s_factor; s++)
			{
				tilted_phase_part2[s] += phase_response[l * cpm->s_factor +s] * U_n;
				tilted_phase_part3[s] += phase_response[l * cpm->s_factor +s];
			}
		}

		for (auto s = 0; s < cpm->s_factor; s++)
		{
			R tilted_phase = tilted_phase_part1 + (R)M_PI * cpm_h * (4 * tilted_phase_part2[s] +
			                 (cpm->m_order -1) * (s * T_samp + (cpm->L -1) - 2 * tilted_phase_part3[s]));

			baseband[allowed_wa * cpm->s_factor + s                      ] = std::cos(tilted_phase);
			baseband[allowed_wa * cpm->s_factor + s + baseband.size() / 2] = std::sin(tilted_phase);
		}
	}
}

template <typename R>
class GMSK
{
private:
	const R off;
	const R factor;

public:
	GMSK(const R b, const R offset)
	: off(offset),
	  factor((R)2.0 * (R)M_PI * b / std::sqrt((R)2.0 * std::log((R)2.0)))
	{ }

	R apply(const R& t) const
	{
		const R x      = t + off;
		const R minus  = (x - (R)0.5) * factor;
		const R plus   = (x + (R)0.5) * factor;

		return tools::div4(std::erf(plus) - std::erf(minus));
	}

	R operator()(const R& t) const
	{
		return apply(t);
	}
};

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
R Modem_CPM<B,R,Q,MAX>
::calculate_phase_response(const R t_stamp)
{
	if (cpm->wave_shape == "GMSK")
	{
		if (t_stamp == (R)0.0)
			return (R)0.0;

		GMSK<R> g((R)0.3, -(R)cpm->L / (R)2.0);
		return tools::mid_rect_integral_seq(g, (R)0.0, t_stamp, (int)(t_stamp / (R)1e-4));
	}
	else if (cpm->wave_shape == "RCOS")
		return t_stamp / ((R)2.0 * cpm->L) - sin((R)2.0 * (R)M_PI * t_stamp / (R)cpm->L) / (R)4.0 / (R)M_PI;
	else if (cpm->wave_shape == "REC")
		return t_stamp / ((R)2.0 * cpm->L);
	else
	{
		std::string message = "Unknown CPM wave shape ('cpm->wave_shape' = " + cpm->wave_shape + ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message);
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_CPM<B,R,Q,MAX>
::generate_projection(const R factor)
{
	if (projection.size() != baseband.size())
	{
		std::stringstream message;
		message << "'projection.size()' and 'baseband.size()' have to be equal ('projection.size()' = "
		        << projection.size() << ", 'baseband.size()' = " << baseband.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (cpm->filters_type == "TOTAL")
	{
		tools::complex_transpose(cpm->max_wa_id, cpm->s_factor, baseband, projection);

		for (auto i = 0; i < (int)projection.size() ; i++)
			projection[i] *= factor;
	}
	//else if(filters_type == "ORTHO_NORM")
	//{
	//}
	else
	{
		std::string message = "Unknown CPM filter bank type ('cpm->filters_type' = " + cpm->filters_type + ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message);
	}
}
}
}
