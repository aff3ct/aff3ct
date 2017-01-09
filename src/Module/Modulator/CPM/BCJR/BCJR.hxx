/*
 * BCJR.hxx
 *
 *  Created on: 14 dec. 2016
 *      Author: ohartmann
 */

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctgmath>
#include <exception>

#include "Tools/Math/utils.h"

#include "BCJR.hpp"
#include "BCJR_tools.hpp"


template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
BCJR<S_in,S_out,Q,MAX>
::BCJR(const CPM_parameters<S_in,S_out>& _cpm, const int _frame_size)
: cpm              (_cpm                               ),
  frame_size       (_frame_size                        ),

  symb_apriori_prob(frame_size                 * cpm.Mo),
  gamma            (frame_size * cpm.max_st_id * cpm.Mo),
  alpha            (frame_size * cpm.max_st_id         ),
  beta             (frame_size * cpm.max_st_id         ),
  proba_msg_symb   (frame_size                 * cpm.Mo),
  proba_msg_bits   (frame_size * cpm.Nb_b_per_s* 2     )
{
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
BCJR<S_in,S_out,Q,MAX>
::~BCJR()
{
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::decode(const mipp::vector<Q> &Lch_N, mipp::vector<Q> &Le_N)
{
	std::fill(symb_apriori_prob.begin(), symb_apriori_prob.end(), (Q)0);

	compute_alpha_beta_gamma(Lch_N);
	symboles_probas         (     );
	bits_probas             (     );
	compute_ext             (Le_N );
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::decode(const mipp::vector<Q> &Lch_N, const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N)
{
	LLR_to_logsymb_proba    (Ldec_N      );
	compute_alpha_beta_gamma(Lch_N       );
	symboles_probas         (            );
	bits_probas             (            );
	compute_ext             (Ldec_N, Le_N);

//	std::cout << "allowed_wave_forms : "  << std::endl;
//
//	for (auto wa = 0; wa < cpm.Nb_wa; wa++)
//		std::cout << cpm.allowed_wave_forms[wa] << " ";
//	std::cout << std::endl;
//	std::cout << std::endl;
//
//	std::cout << "allowed_states : "  << std::endl;
//
//	for (auto wa = 0; wa < cpm.Nb_st; wa++)
//		std::cout << cpm.allowed_states[wa] << " ";
//	std::cout << std::endl;
//	std::cout << std::endl;
//
//	std::cout << "Lch_N : "  << std::endl;
//
//	for (auto i = 0; i < 20; i++)
//	{
//		for (auto wa = 0; wa < cpm.Nb_wa; wa++)
//			std::cout << Lch_N[i*cpm.max_wa_id + cpm.allowed_wave_forms[wa]]/2 << " ";
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//
//	std::cout << "Lch_N2 : "  << std::endl;
//
//	for (auto i = 0; i < 20; i++)
//	{
//		for (auto wa = 0; wa < cpm.max_wa_id; wa++)
//			std::cout << Lch_N[i*cpm.max_wa_id + wa]/2 << " ";
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//
//	std::cout << "Ldec_N : "  << std::endl;
//
//	for (auto i = 0; i < 20; i++)
//	{
//			std::cout << Ldec_N[i] << " ";
//		std::cout <<  "| ";
//	}
//	std::cout << std::endl;
//	std::cout << std::endl;
//
//	std::cout << "gamma : " << std::endl ;
//
//	for (auto i = 0; i < 20; i++)
//	{
//		for (auto st = 0; st < cpm.Nb_st; st++)
//		{
//			for (auto tr = 0; tr < cpm.Mo; tr++)
//				std::cout << gamma[(tr * cpm.max_st_id + cpm.allowed_states[st]) * frame_size +i] << " ";
//
//			std::cout << "| ";
//		}
//		std::cout <<"|"<< std::endl;
//	}
//	std::cout << std::endl;
//
//	std::cout << "alpha : " << std::endl ;
//
//	for (auto i = 0; i < 20; i++)
//	{
//		for (auto st = 0; st < cpm.Nb_st; st++)
//				std::cout << alpha[i * cpm.max_st_id +cpm.allowed_states[st]] << " ";
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//
//	std::cout << "beta : " << std::endl ;
//
//	for (auto i = 0; i < 20; i++)
//	{
//		for (auto st = 0; st < cpm.Nb_st; st++)
//				std::cout << beta[i * cpm.max_st_id +cpm.allowed_states[st]] << " ";
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//
//	std::cout << "proba_msg_symb : "  << std::endl;
//
//	for (auto i = 0; i < 20; i++)
//	{
//		for (auto j = 0; j < cpm.Mo; j++)
//				std::cout << proba_msg_symb[i * cpm.Mo + j] << " ";
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//
//
//	std::cout << "proba_msg_bits : "  << std::endl;
//
//	for (auto i = 0; i < 20; i++)
//	{
//			std::cout << proba_msg_bits[i] << " ";
//		std::cout <<  "| ";
//	}
//	std::cout << std::endl;
//	std::cout << std::endl;
//
//	std::cout << "Le_N : " << std::endl ;
//
//	for (auto i = 0; i < 20; i++)
//	{
//			std::cout << Le_N[i] << " ";
//		std::cout <<  "| ";
//	}
//	std::cout << std::endl;
//	std::cout << std::endl;

}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::LLR_to_logsymb_proba(const mipp::vector<Q> &Ldec_N)
{
	assert(Ldec_N.size() == (symb_apriori_prob.size()/cpm.Mo - cpm.tl) * cpm.Nb_b_per_s);

	std::fill(symb_apriori_prob.begin(), symb_apriori_prob.end(), (Q)0);

	for (int i = 0; i < (int)Ldec_N.size()/cpm.Nb_b_per_s; i++)
		for (int tr = 0; tr < cpm.Mo; tr++)
		{
			for (int b = 0; b < cpm.Nb_b_per_s; b++)
			{
				const int bit_state = cpm.transition_to_binary[tr * cpm.Nb_b_per_s + b]; // transition_to_binary what bit state we should have for the given transition and bit position
				const int sign		= (bit_state == 0) ? 1 : -1;   //associated coeff
				symb_apriori_prob[i * cpm.Mo + tr] += (Q)sign*Ldec_N[i * cpm.Nb_b_per_s + b]/(Q)2; //match -> add prob else remove
			}
		}
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::compute_alpha_beta_gamma(const mipp::vector<Q> &Lch_N)
{
	// alpha and beta initialization
	std::fill(alpha.begin(), alpha.end(), negative_inf<Q>());
	std::fill(beta .begin(), beta .end(), negative_inf<Q>());
	alpha[                                 cpm.allowed_states[0]] = 0;
	beta [(frame_size -1) * cpm.max_st_id +cpm.allowed_states[0]] = 0;

	// compute gamma
	for (auto i = 0; i < frame_size; i++)
		for (auto st = 0; st < cpm.Nb_st; st++)
			for (auto tr = 0; tr < cpm.Mo; tr++)
				gamma[(i * cpm.max_st_id + cpm.allowed_states[st]) * cpm.Mo + tr] =
					Lch_N[i*cpm.max_wa_id + cpm.trellis_related_wave_form[cpm.allowed_states[st] * cpm.Mo +tr]] // info from the channel
					+ symb_apriori_prob[i * cpm.Mo +tr];                     			  					    // info from the decoder


	// compute alpha and beta
	for (auto i = 1; i < frame_size; i++)
	{
		for (auto st = 0; st < cpm.Nb_st; st++)
		{
			// compute the alpha nodes
			for (auto tr = 0; tr < cpm.Mo; tr++)
				alpha        [(i -0) * cpm.max_st_id + cpm.allowed_states[st]] =
				    MAX(alpha[(i -0) * cpm.max_st_id + cpm.allowed_states[st]],
				        alpha[(i -1) * cpm.max_st_id + cpm.anti_trellis_original_state[cpm.allowed_states[st]*cpm.Mo +tr]] +
				        gamma[((i-1) * cpm.max_st_id + cpm.anti_trellis_original_state[cpm.allowed_states[st]*cpm.Mo +tr]) * cpm.Mo
							  + cpm.anti_trellis_input_transition[cpm.allowed_states[st]*cpm.Mo +tr]]);

			// compute the beta nodes
			for (auto tr = 0; tr < cpm.Mo; tr++)
				beta         [(frame_size - (i +1)) * cpm.max_st_id + cpm.allowed_states[st]] =
				    MAX(beta [(frame_size - (i +1)) * cpm.max_st_id + cpm.allowed_states[st]],
				        beta [(frame_size - (i +0)) * cpm.max_st_id + cpm.trellis_next_state[cpm.allowed_states[st]*cpm.Mo +tr]] +
						gamma[((frame_size-      i) * cpm.max_st_id + cpm.allowed_states[st]) * cpm.Mo + tr]);
		}

		// normalize alpha and beta vectors (not impact on the decoding performances)
		BCJR_normalize<Q,MAX>(&alpha[              (i +0)  * cpm.max_st_id], i, cpm.max_st_id);
		BCJR_normalize<Q,MAX>(&beta [(frame_size - (i +1)) * cpm.max_st_id], i, cpm.max_st_id);
	}
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::symboles_probas()
{
	std::fill(proba_msg_symb.begin(), proba_msg_symb.end(), negative_inf<Q>());

	for (auto i = 0; i < frame_size; i++)
		for (auto tr = 0; tr < cpm.Mo; tr++)
			for (auto st = 0; st < cpm.Nb_st; st++)
				proba_msg_symb        [ i * cpm.Mo  + tr] =
				    MAX(proba_msg_symb[ i * cpm.Mo  + tr],
				        alpha   [ i * cpm.max_st_id + cpm.allowed_states[st]] +
				        beta    [ i * cpm.max_st_id + cpm.trellis_next_state[cpm.allowed_states[st]*cpm.Mo + tr]] +
						gamma   [(i * cpm.max_st_id + cpm.allowed_states[st]) * cpm.Mo + tr]);
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::bits_probas()
{
	// initialize proba_msg_bits
	std::fill(proba_msg_bits.begin(), proba_msg_bits.end(), negative_inf<Q>());

	for (auto i = 0; i < frame_size; i++)
	{
		for (auto b = 0; b < cpm.Nb_b_per_s; b++)
			for (auto tr = 0; tr < cpm.Mo; tr++)
			{
				const int bit_state = cpm.transition_to_binary[tr * cpm.Nb_b_per_s + b]; // bit_state = 0 or 1 ; bit 0 is msb, bit cpm.Nb_b_per_s-1 is lsb

				proba_msg_bits[(i*cpm.Nb_b_per_s+b)*2 + bit_state] = MAX(proba_msg_bits[(i*cpm.Nb_b_per_s+b)*2 + bit_state],
																		 proba_msg_symb[i*cpm.Mo +tr]);
			}
	}
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
::compute_ext(mipp::vector<Q> &Le_N)
{
	assert(Le_N.size()*2 <= proba_msg_bits.size());

	// remove tail bits because Le_N.size*2 <= proba_msg_bits.size + modulation tail bits
	for (auto i = 0; i < (int)Le_N.size(); i ++)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i] = proba_msg_bits[i*2] - proba_msg_bits[i*2 +1];
}

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
void BCJR<S_in,S_out,Q,MAX>
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
