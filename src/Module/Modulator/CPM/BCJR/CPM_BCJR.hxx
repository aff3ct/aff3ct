#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctgmath>
#include <exception>

#include "Tools/Math/utils.h"

#include "CPM_BCJR.hpp"

template<typename Q>
inline Q negative_inf(){return -std::numeric_limits<Q>::max(); }

template<>
inline short negative_inf<short>(){return -(1 << (sizeof(short) * 8 -2)); }

template<>
inline signed char negative_inf<signed char>(){return -63; }

template<typename Q>
inline Q positive_inf(){return std::numeric_limits<Q>::max(); }

template<>
inline short positive_inf<short>(){return (1 << (sizeof(short) * 8 -2)); }

template<>
inline signed char positive_inf<signed char>(){return 63; }

template <typename Q, proto_max<Q> MAX>
inline void BCJR_normalize(Q *metrics, const int &i, const int &n_states)
{
	// normalization
	auto norm_val = negative_inf<Q>();
	for (auto j = 0; j < n_states; j++)
		norm_val = MAX(norm_val, metrics[j]);

	for (auto j = 0; j < n_states; j++)
		metrics[j] -= norm_val;
}

template <signed char, proto_max<signed char> MAX>
inline void BCJR_normalize(signed char *metrics, const int &i, const int &n_states)
{
	// normalization
	auto norm_val = negative_inf<signed char>();
	for (auto j = 0; j < n_states; j++)
		norm_val = MAX(norm_val, metrics[j]);

	for (auto j = 0; j < n_states; j++)
		metrics[j] = saturate<signed char>(metrics[j] - norm_val, -63, +63);
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
CPM_BCJR<SIN,SOUT,Q,MAX>
::CPM_BCJR(const CPM_parameters<SIN,SOUT>& _cpm, const int _frame_size)
: cpm              (_cpm                                    ),
  frame_size       (_frame_size                             ),

  symb_apriori_prob(frame_size                 * cpm.m_order),
  gamma            (frame_size * cpm.max_st_id * cpm.m_order),
  alpha            (frame_size * cpm.max_st_id              ),
  beta             (frame_size * cpm.max_st_id              ),
  proba_msg_symb   (frame_size                 * cpm.m_order),
  proba_msg_bits   (frame_size * cpm.n_b_per_s* 2           )
{
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
CPM_BCJR<SIN,SOUT,Q,MAX>
::~CPM_BCJR()
{
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::decode(const mipp::vector<Q> &Lch_N, mipp::vector<Q> &Le_N)
{
	std::fill(symb_apriori_prob.begin(), symb_apriori_prob.end(), (Q)0);

	compute_alpha_beta_gamma(Lch_N);
	symboles_probas         (     );
	bits_probas             (     );
	compute_ext             (Le_N );
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::decode(const mipp::vector<Q> &Lch_N, const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N)
{
	LLR_to_logsymb_proba    (Ldec_N      );
	compute_alpha_beta_gamma(Lch_N       );
	symboles_probas         (            );
	bits_probas             (            );
	compute_ext             (Ldec_N, Le_N);
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::LLR_to_logsymb_proba(const mipp::vector<Q> &Ldec_N)
{
	assert(Ldec_N.size() == (symb_apriori_prob.size()/cpm.m_order - cpm.tl) * cpm.n_b_per_s);

	std::fill(symb_apriori_prob.begin(), symb_apriori_prob.end(), (Q)0);

	for (int i = 0; i < (int)Ldec_N.size()/cpm.n_b_per_s; i++)
		for (int tr = 0; tr < cpm.m_order; tr++)
		{
			for (int b = 0; b < cpm.n_b_per_s; b++)
			{
				const int bit_state = cpm.transition_to_binary[tr * cpm.n_b_per_s + b]; // transition_to_binary what bit state we should have for the given transition and bit position
				const int sign        = (bit_state == 0) ? 1 : -1; //associated coeff
				symb_apriori_prob[i * cpm.m_order + tr] += (Q)sign*Ldec_N[i * cpm.n_b_per_s + b]/(Q)2; //match -> add prob else remove
			}
		}
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::compute_alpha_beta_gamma(const mipp::vector<Q> &Lch_N)
{
    // alpha and beta initialization
	std::fill(alpha.begin(), alpha.end(), negative_inf<Q>());
	std::fill(beta .begin(), beta .end(), negative_inf<Q>());
	alpha[                                 cpm.allowed_states[0]] = 0;
	beta [(frame_size -1) * cpm.max_st_id +cpm.allowed_states[0]] = 0;

	// compute gamma
	for (auto i = 0; i < frame_size; i++)
		for (auto st = 0; st < cpm.n_st; st++)
			for (auto tr = 0; tr < cpm.m_order; tr++)
				gamma[(i * cpm.max_st_id + cpm.allowed_states[st]) * cpm.m_order + tr] =
				      Lch_N[i*cpm.max_wa_id + cpm.trellis_related_wave_form[cpm.allowed_states[st] * cpm.m_order +tr]] // info from the channel
				      + symb_apriori_prob[i * cpm.m_order +tr];                                                        // info from the decoder


	// compute alpha and beta
	for (auto i = 1; i < frame_size; i++)
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
				beta         [(frame_size - (i +1)) * cpm.max_st_id + cpm.allowed_states[st]] =
				    MAX(beta [(frame_size - (i +1)) * cpm.max_st_id + cpm.allowed_states[st]],
				        beta [(frame_size - (i +0)) * cpm.max_st_id + cpm.trellis_next_state[cpm.allowed_states[st]*cpm.m_order +tr]] +
				        gamma[((frame_size-      i) * cpm.max_st_id + cpm.allowed_states[st]) * cpm.m_order + tr]);
		}

		// normalize alpha and beta vectors (not impact on the decoding performances)
		BCJR_normalize<Q,MAX>(&alpha[              (i +0)  * cpm.max_st_id], i, cpm.max_st_id);
		BCJR_normalize<Q,MAX>(&beta [(frame_size - (i +1)) * cpm.max_st_id], i, cpm.max_st_id);
	}
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::symboles_probas()
{
	std::fill(proba_msg_symb.begin(), proba_msg_symb.end(), negative_inf<Q>());

	for (auto i = 0; i < frame_size; i++)
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

	for (auto i = 0; i < frame_size; i++)
	{
		for (auto b = 0; b < cpm.n_b_per_s; b++)
			for (auto tr = 0; tr < cpm.m_order; tr++)
			{
				const int bit_state = cpm.transition_to_binary[tr * cpm.n_b_per_s + b]; // bit_state = 0 or 1 ; bit 0 is msb, bit cpm.n_b_per_s-1 is lsb

				proba_msg_bits[(i*cpm.n_b_per_s+b)*2 + bit_state] = MAX(proba_msg_bits[(i*cpm.n_b_per_s+b)*2 + bit_state],
				                                                         proba_msg_symb[i*cpm.m_order +tr]);
			}
	}
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::compute_ext(mipp::vector<Q> &Le_N)
{
	assert(Le_N.size()*2 <= proba_msg_bits.size());

	// remove tail bits because Le_N.size*2 <= proba_msg_bits.size + modulation tail bits
	for (auto i = 0; i < (int)Le_N.size(); i ++)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i] = proba_msg_bits[i*2] - proba_msg_bits[i*2 +1];
}

template <typename SIN, typename SOUT,  typename Q, proto_max<Q> MAX>
void CPM_BCJR<SIN,SOUT,Q,MAX>
::compute_ext(const mipp::vector<Q> &Ldec_N,
                    mipp::vector<Q> &Le_N)
{
	assert(Le_N.size()*2 <= proba_msg_bits.size());
	assert(Le_N.size()   == Ldec_N.size()        );

	// remove tail bits because Le_N.size*2 <= proba_msg_bits.size + modulation tail bits
	for (auto i = 0; i < (int)Le_N.size(); i ++)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i] = proba_msg_bits[i*2] - (proba_msg_bits[i*2+1] + Ldec_N[i]);
}
