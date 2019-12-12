#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <sstream>
#include <limits>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/CPM/BCJR/CPM_BCJR.hpp"

namespace aff3ct
{
namespace tools
{
template<typename Q>
Q negative_inf(){return std::numeric_limits<Q>::lowest(); }

template<typename Q>
Q positive_inf(){return std::numeric_limits<Q>::max(); }

template <typename Q, proto_max<Q> MAX>
void BCJR_normalize(Q *metrics, const int &n_states)
{
	// normalization
	auto norm_val = negative_inf<Q>();
	for (auto j = 0; j < n_states; j++)
		norm_val = MAX(norm_val, metrics[j]);

	for (auto j = 0; j < n_states; j++)
		metrics[j] -= norm_val;
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
CPM_BCJR<SIN,SOUT,Q,MAX>
::CPM_BCJR(const CPM_parameters<SIN,SOUT>& _cpm, const int _n_symbols)
: cpm              (_cpm                                   ),
  n_symbols        (_n_symbols                             ),
  chn_size         ( n_symbols           * cpm.max_wa_id   ),
  dec_size         ((n_symbols - cpm.tl) * cpm.n_b_per_s   ),
  ext_size         ( dec_size                              ),

  symb_apriori_prob(n_symbols                 * cpm.m_order),
  gamma            (n_symbols * cpm.max_st_id * cpm.m_order),
  alpha            (n_symbols * cpm.max_st_id              ),
  beta             (n_symbols * cpm.max_st_id              ),
  proba_msg_symb   (n_symbols                 * cpm.m_order),
  proba_msg_bits   (n_symbols * cpm.n_b_per_s * 2          )
{
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::decode(const std::vector<Q> &Lch_N, std::vector<Q> &Le_N)
{
	if (Lch_N.size() != chn_size)
	{
		std::stringstream message;
		message << "'Lch_N.size()' has to be equal to 'chn_size' ('Lch_N.size()' = " << Lch_N.size()
		        << ", 'chn_size' = " << chn_size << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (Le_N.size() != ext_size)
	{
		std::stringstream message;
		message << "'Le_N.size()' has to be equal to 'ext_size' ('Le_N.size()' = " << Le_N.size()
		        << ", 'ext_size' = " << ext_size << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decode(Lch_N.data(), Le_N.data());
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::decode(const Q *Lch_N, Q *Le_N)
{
	std::fill(symb_apriori_prob.begin(), symb_apriori_prob.end(), (Q)0);

	compute_alpha_beta_gamma(Lch_N);
	symboles_probas         (     );
	bits_probas             (     );
	compute_ext             (Le_N );
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::decode(const std::vector<Q> &Lch_N, const std::vector<Q> &Ldec_N, std::vector<Q> &Le_N)
{
	if (Lch_N.size() != chn_size)
	{
		std::stringstream message;
		message << "'Lch_N.size()' has to be equal to 'chn_size' ('Lch_N.size()' = " << Lch_N.size()
		        << ", 'chn_size' = " << chn_size << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (Ldec_N.size() != dec_size)
	{
		std::stringstream message;
		message << "'Ldec_N.size()' has to be equal to 'dec_size' ('Ldec_N.size()' = " << Ldec_N.size()
		        << ", 'dec_size' = " << dec_size << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (Le_N.size() != ext_size)
	{
		std::stringstream message;
		message << "'Le_N.size()' has to be equal to 'ext_size' ('Le_N.size()' = " << Le_N.size()
		        << ", 'ext_size' = " << ext_size << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decode(Lch_N.data(), Ldec_N.data(), Le_N.data());
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::decode(const Q *Lch_N, const Q *Ldec_N, Q *Le_N)
{
	LLR_to_logsymb_proba    (Ldec_N      );
	compute_alpha_beta_gamma(Lch_N       );
	symboles_probas         (            );
	bits_probas             (            );
	compute_ext             (Ldec_N, Le_N);
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::LLR_to_logsymb_proba(const Q *Ldec_N)
{
	std::fill(symb_apriori_prob.begin(), symb_apriori_prob.end(), (Q)0);

	for (int i = 0; i < dec_size/cpm.n_b_per_s; i++)
		for (int tr = 0; tr < cpm.m_order; tr++)
			for (int b = 0; b < cpm.n_b_per_s; b++)
			{
				// transition_to_binary what bit state we should have for the given transition and bit position
				const int bit_state = (int)cpm.transition_to_binary[tr * cpm.n_b_per_s + b];
				// match -> add probability else remove
				symb_apriori_prob[i * cpm.m_order + tr] += (bit_state == 0) ?  Ldec_N[i * cpm.n_b_per_s + b]/2
				                                                            : -Ldec_N[i * cpm.n_b_per_s + b]/2;
			}
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::compute_alpha_beta_gamma(const Q *Lch_N)
{
	// alpha and beta initialization
	std::fill(alpha.begin(), alpha.end(), negative_inf<Q>());
	std::fill(beta .begin(), beta .end(), negative_inf<Q>());
	alpha[                                cpm.allowed_states[0]] = 0;
	beta [(n_symbols -1) * cpm.max_st_id +cpm.allowed_states[0]] = 0;

	// compute gamma
	for (auto i = 0; i < n_symbols; i++)
		for (auto st = 0; st < cpm.n_st; st++)
			for (auto tr = 0; tr < cpm.m_order; tr++)
				gamma[(i * cpm.max_st_id + cpm.allowed_states[st]) * cpm.m_order + tr] =
				      Lch_N[i*cpm.max_wa_id + cpm.trellis_related_wave_form[cpm.allowed_states[st] * cpm.m_order +tr]] // info from the channel
				      + symb_apriori_prob[i * cpm.m_order +tr];                                                        // info from the decoder


	// compute alpha and beta
	for (auto i = 1; i < n_symbols; i++)
	{
		for (auto st = 0; st < cpm.n_st; st++)
		{
			// compute the alpha nodes
			for (auto tr = 0; tr < cpm.m_order; tr++)
				alpha        [(i -0) * cpm.max_st_id + cpm.allowed_states[st]] =
				    MAX(alpha[(i -0) * cpm.max_st_id + cpm.allowed_states[st]],
				        alpha[(i -1) * cpm.max_st_id + cpm.anti_trellis_original_state[cpm.allowed_states[st]*cpm.m_order +tr]] +
				        gamma[((i-1) * cpm.max_st_id + cpm.anti_trellis_original_state[cpm.allowed_states[st]*cpm.m_order +tr]) * cpm.m_order
				              + cpm.anti_trellis_input_transition[cpm.allowed_states[st]*cpm.m_order +tr]]);

			// compute the beta nodes
			for (auto tr = 0; tr < cpm.m_order; tr++)
				beta         [(n_symbols - (i +1)) * cpm.max_st_id + cpm.allowed_states[st]] =
				    MAX(beta [(n_symbols - (i +1)) * cpm.max_st_id + cpm.allowed_states[st]],
				        beta [(n_symbols - (i +0)) * cpm.max_st_id + cpm.trellis_next_state[cpm.allowed_states[st]*cpm.m_order +tr]] +
				        gamma[((n_symbols-      i) * cpm.max_st_id + cpm.allowed_states[st]) * cpm.m_order + tr]);
		}

		// normalize alpha and beta vectors (not impact on the decoding performances)
		BCJR_normalize<Q,MAX>(&alpha[             (i +0)  * cpm.max_st_id], cpm.max_st_id);
		BCJR_normalize<Q,MAX>(&beta [(n_symbols - (i +1)) * cpm.max_st_id], cpm.max_st_id);
	}
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::symboles_probas()
{
	std::fill(proba_msg_symb.begin(), proba_msg_symb.end(), negative_inf<Q>());

	for (auto i = 0; i < n_symbols; i++)
		for (auto tr = 0; tr < cpm.m_order; tr++)
			for (auto st = 0; st < cpm.n_st; st++)
				proba_msg_symb        [ i * cpm.m_order  + tr] =
				    MAX(proba_msg_symb[ i * cpm.m_order  + tr],
				        alpha   [ i * cpm.max_st_id + cpm.allowed_states[st]] +
				        beta    [ i * cpm.max_st_id + cpm.trellis_next_state[cpm.allowed_states[st]*cpm.m_order + tr]] +
				        gamma   [(i * cpm.max_st_id + cpm.allowed_states[st]) * cpm.m_order + tr]);
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::bits_probas()
{
	// initialize proba_msg_bits
	std::fill(proba_msg_bits.begin(), proba_msg_bits.end(), negative_inf<Q>());

	for (auto i = 0; i < n_symbols; i++)
	{
		for (auto b = 0; b < cpm.n_b_per_s; b++)
			for (auto tr = 0; tr < cpm.m_order; tr++)
			{
				const auto bit_state = cpm.transition_to_binary[tr * cpm.n_b_per_s + b]; // bit_state = 0 or 1 ; bit 0 is msb, bit cpm.n_b_per_s-1 is lsb

				proba_msg_bits[(i*cpm.n_b_per_s+b)*2 + bit_state] = MAX(proba_msg_bits[(i*cpm.n_b_per_s+b)*2 + bit_state],
				                                                        proba_msg_symb[i*cpm.m_order +tr]);
			}
	}
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::compute_ext(Q *Le_N)
{
	// remove tail bits
	for (auto i = 0; i < ext_size; i ++)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i] = proba_msg_bits[i*2] - proba_msg_bits[i*2 +1];
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::compute_ext(const Q *Ldec_N, Q *Le_N)
{
	// remove tail bits
	for (auto i = 0; i < ext_size; i ++)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i] = proba_msg_bits[i*2] - (proba_msg_bits[i*2 +1] + Ldec_N[i]);
}
}
}
