#ifndef MODULATOR_GSM_BCJR_HPP_
#define MODULATOR_GSM_BCJR_HPP_

#include <vector>
#include "Tools/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Math/max.h"

#include "../../Modulator.hpp"

template <typename Q, proto_max<Q> MAX>
class Modulator_GSM_BCJR
{
protected:
	const int frame_size; // same as previous "cpm_symb_message_length" variable
	const int n_states;
	const int m_order;
	const int n_bits_per_symb;
	const int n_prev_branches;

	const std::vector<int> binary_symbols;
	const std::vector<int> trellis;
	const std::vector<int> anti_trellis;

	mipp::vector<Q> symb_apriori_prob;
	mipp::vector<Q> gamma;
	mipp::vector<Q> alpha;
	mipp::vector<Q> beta;
	mipp::vector<Q> proba_msg_symb;
	mipp::vector<Q> proba_msg_bits;

public:
	Modulator_GSM_BCJR(const int frame_size, 
	                   const int n_states, 
	                   const int m_order, 
	                   const int n_bits_per_symb, 
	                   const int n_prev_branches, 
	                   const std::vector<int> binary_symbols, 
	                   const std::vector<int> trellis, 
	                   const std::vector<int> anti_trellis);
	virtual ~Modulator_GSM_BCJR();

	// BCJR for the demodulation
	void decode(const mipp::vector<Q> &Lch_N,                                mipp::vector<Q> &Le_N);
	void decode(const mipp::vector<Q> &Lch_N, const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N);

private:
	void LLR_to_logsymb_proba    (const mipp::vector<Q> &Ldec_N                       );
	void compute_alpha_beta_gamma(const mipp::vector<Q> &Lch_N                        );
	void symboles_probas         (                                                    );
	void bits_probas             (                                                    );
	void compute_ext             (                               mipp::vector<Q> &Le_N);
	void compute_ext             (const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N);
};

#include "Modulator_GSM_BCJR.hxx"

#endif /* MODULATOR_GSM_BCJR_HPP_ */
