#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "../../../Decoder/decoder_functions.h"

#include "Modulator_GSM_BCJR.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BCJR tools //////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Q>
struct negative_inf 
{
	static Q get() { return -std::numeric_limits<Q>::max(); } 
};

template <>
struct negative_inf <short>
{
	static short get() { return -(1 << (sizeof(short) * 8 -2)); }
};

template <>
struct negative_inf <signed char>
{
	static signed char get() { return -63; }
};

template <typename Q, proto_max<Q> MAX>
struct BCJR_normalize
{
	// Adrien comment's: I think that the normalisation is useless in floating point arithmetic
	static void apply(Q *metrics, const int &i, const int &n_states)
	{
		// normalization
		auto norm_val = negative_inf<Q>::get();
		for (auto j = 0; j < n_states; j++)
			norm_val = MAX(norm_val, metrics[j]);

		for (auto j = 0; j < n_states; j++)
			metrics[j] -= norm_val;
	}
};

template <proto_max<signed char> MAX>
struct BCJR_normalize <signed char,MAX>
{
	static void apply(signed char *metrics, const int &i, const int &n_states)
	{
		// normalization & saturation
		auto norm_val = negative_inf<signed char>::get();
		for (auto j = 0; j < n_states; j++)
			norm_val = MAX(norm_val, metrics[j]);

		for (auto j = 0; j < n_states; j++)
			metrics[j] = saturate<signed char>(metrics[j] - norm_val, -63, +63);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Q, proto_max<Q> MAX>
Modulator_GSM_BCJR<Q,MAX>
::Modulator_GSM_BCJR(const int frame_size, 
	                 const int n_states, 
	                 const int m_order, 
	                 const int n_bits_per_symb, 
	                 const int n_prev_branches, 
	                 const std::vector<int> binary_symbols, 
	                 const std::vector<int> trellis, 
	                 const std::vector<int> anti_trellis)
: frame_size     (frame_size     ), 
  n_states       (n_states       ), 
  m_order        (m_order        ), 
  n_bits_per_symb(n_bits_per_symb), 
  n_prev_branches(n_prev_branches),
  
  binary_symbols(binary_symbols),
  trellis       (trellis       ),
  anti_trellis  (anti_trellis  ),

  symb_apriori_prob(frame_size            * m_order   ),
  gamma            (frame_size * n_states * m_order   ),
  alpha            (frame_size * n_states             ),
  beta             (frame_size * n_states             ),
  proba_msg_symb   (frame_size            * m_order, 0),
  proba_msg_bits   (frame_size            * 2      , 0)
{
}

template <typename Q, proto_max<Q> MAX>
Modulator_GSM_BCJR<Q,MAX>
::~Modulator_GSM_BCJR()
{
}

template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::decode(const mipp::vector<Q> &Lch_N, mipp::vector<Q> &Le_N)
{
	std::fill(this->symb_apriori_prob.begin(), this->symb_apriori_prob.end(), 0);

	compute_alpha_beta_gamma(Lch_N);
	symboles_probas         (     );
	bits_probas             (     );
	compute_ext             (Le_N );
}

template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::decode(const mipp::vector<Q> &Lch_N, const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N)
{
	LLR_to_logsymb_proba    (Ldec_N      );
	compute_alpha_beta_gamma(Lch_N       );
	symboles_probas         (            );
	bits_probas             (            );
	compute_ext             (Ldec_N, Le_N);
}

// retrieve log symbols probability from LLR
template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::LLR_to_logsymb_proba(const mipp::vector<Q> &Ldec_N)
{
	assert(Ldec_N.size() <= this->symb_apriori_prob.size() / m_order);

	std::fill(this->symb_apriori_prob.begin(), this->symb_apriori_prob.end(), 0);

	for (auto i = 0; i < (int)Ldec_N.size(); i++)
		for (auto j = 0; j < this->m_order; j++)
			for (auto k = 0; k < this->n_bits_per_symb; k++)
			{
				const auto symbol = this->binary_symbols[k * this->m_order +j];
				const auto sign   = 1 - (symbol + symbol);
				this->symb_apriori_prob[i * this->m_order +j] += (Q)sign * Ldec_N[i * this->n_bits_per_symb +k] * 0.5;
			}
}

// compute gamma, alpha and beta, heart of the processing
template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::compute_alpha_beta_gamma(const mipp::vector<Q> &Lch_N)
{
	// alpha and beta initialization
	std::fill(this->alpha.begin(), this->alpha.end(), negative_inf<Q>::get());
	std::fill(this->beta .begin(), this->beta .end(), negative_inf<Q>::get());
	this->alpha[0                                     ] = 0;
	this->beta [(this->frame_size -1) * this->n_states] = 0;

	// other way to initialize beta: does not support the fixed point mode because of the log function call
	// this is usefull when we don't use the tail bits in the turbo demodulation
	// auto beta_final_equi_proba = 1.0 / this->n_states;
	for (auto i = 0; i < this->n_states; i++)
		// this->beta[(this->frame_size -1) * n_states +i] = std::log(beta_final_equi_proba);
		this->beta[(this->frame_size -1) * n_states +i] = 0;

	// compute gamma
	for (auto i = 0; i < this->frame_size; i++)
		for (auto j = 0; j < this->n_states; j++)
			for (auto k = 0; k < this->m_order; k++)
				this->gamma[(k * n_states +j) * this->frame_size +i] = 
					Lch_N[this->trellis[(k + this->m_order) * this->n_states +j] * this->frame_size +i] + // info from the channel
					this->symb_apriori_prob[i * this->m_order +k];                                        // info from the decoder

	// compute alpha and beta
	for (auto i = 1; i < this->frame_size; i++)
	{
		for (auto j = 0; j < this->n_states; j++)
		{
			const auto original_state = this->anti_trellis.data() +                  j  * this->n_prev_branches;
			const auto input_symb     = this->anti_trellis.data() + (this->n_states +j) * this->n_prev_branches;

			// compute the alpha nodes
			for (auto k = 0; k < this->n_prev_branches; k++)
				this->alpha        [(i -0) * this->n_states                  +j] = 
				    MAX(this->alpha[(i -0) * this->n_states                  +j],
				        this->alpha[(i -1) * this->n_states + original_state[k]] +
				        this->gamma[(input_symb[k] * this->n_states + original_state[k]) * this->frame_size +i -1]);

			// compute the beta nodes
			for (auto k = 0; k < this->m_order; k++)
				this->beta         [(this->frame_size - (i +1)) * this->n_states                                     +j] = 
				    MAX(this->beta [(this->frame_size - (i +1)) * this->n_states                                     +j],
				        this->beta [(this->frame_size - (i +0)) * this->n_states + this->trellis[k * this->n_states +j]] +
				        this->gamma[(k * this->n_states +j) * this->frame_size + this->frame_size -i]);
		}

		// normalize alpha and beta vectors (not impact on the decoding performances)
		BCJR_normalize<Q,MAX>::apply(&this->alpha[                    (i +0)  * this->n_states], i, this->n_states);
		BCJR_normalize<Q,MAX>::apply(&this->beta [(this->frame_size - (i +1)) * this->n_states], i, this->n_states);
	}
}

// from alpha, beta, and gamma computes new symbol probability
template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::symboles_probas()
{
	// initialize proba_msg_symb
	for (auto i = 0; i < this->frame_size; i++)
		for (auto j = 0; j < this->m_order; j++)
			proba_msg_symb[i * this->m_order +j] = negative_inf<Q>::get();

	for (auto i = 0; i < this->frame_size; i++)
		for (auto j = 0; j < this->m_order; j++)
			for (auto k = 0; k < this->n_states; k++)
				proba_msg_symb        [ i * this->m_order                                      +j] = 
				    MAX(proba_msg_symb[ i * this->m_order                                      +j],
				        this->alpha   [ i * this->n_states                                     +k] +
				        this->beta    [ i * this->n_states + this->trellis[j * this->n_states +k]] +
				        this->gamma   [(j * this->n_states +k) * this->frame_size              +i]);
}

// from symbol probabilities, computes bit probabilities
template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::bits_probas()
{
	// initialize proba_msg_bits
	std::fill(proba_msg_bits.begin(), proba_msg_bits.end(), negative_inf<Q>::get());

	for (auto i = 0; i < this->frame_size; i++)
	{
		const auto ii = i * this->n_bits_per_symb;
		for (auto j = 0; j < n_bits_per_symb; j++)
			for (auto k = 0; k < this->m_order; k++)
			{
				const auto symbol = binary_symbols[j * this->m_order +k];

				// bit 0
				if (symbol == 0) 
					proba_msg_bits[(ii +j) * 2 +0] = MAX(proba_msg_bits[(ii +j) * 2 +0], 
					                                     proba_msg_symb[i * this->m_order +k]);
				// bit 1
				else if (symbol == 1) 
					proba_msg_bits[(ii +j) * 2 +1] = MAX(proba_msg_bits[(ii +j) * 2 +1],
					                                     proba_msg_symb[i * this->m_order +k]);
			}
	}
}

// extrinsic information processing from bit probabilities
template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::compute_ext(mipp::vector<Q> &Le_N)
{
	const auto loop_size = (int)Le_N.size() * 2;
	for (auto i = 0; i < loop_size; i += 2)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i / 2] = proba_msg_bits[i] - proba_msg_bits[i +1];
}

// extrinsic information processing from bit probabilities and CPM a priori LLR
template <typename Q, proto_max<Q> MAX>
void Modulator_GSM_BCJR<Q,MAX>
::compute_ext(const mipp::vector<Q> &Ldec_N,
                    mipp::vector<Q> &Le_N)
{
	const auto loop_size = (int)Le_N.size() * 2;
	for (auto i = 0; i < loop_size; i += 2)
		// processing aposteriori and substracting a priori to directly obtain extrinsic
		Le_N[i / 2] = proba_msg_bits[i] - (proba_msg_bits[i +1] + Ldec_N[i / 2]);
}
