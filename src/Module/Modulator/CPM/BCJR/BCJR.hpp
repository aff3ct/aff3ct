/*
 * BCJR.hpp
 *
 *  Created on: 14 dec. 2016
 *      Author: ohartmann
 */

#ifndef BCJR_HPP_
#define BCJR_HPP_

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"
#include "../CPM_parameters.hpp"

template <typename S_in, typename S_out,  typename Q, proto_max<Q> MAX>
class BCJR
{
protected:
	const CPM_parameters<S_in,S_out>& cpm; 	// all cpm parameters
	const int frame_size;		// size of a frame with tail bits

	mipp::vector<Q> symb_apriori_prob;
	mipp::vector<Q> gamma;
	mipp::vector<Q> alpha;
	mipp::vector<Q> beta;
	mipp::vector<Q> proba_msg_symb;
	mipp::vector<Q> proba_msg_bits;

public:
	BCJR(const CPM_parameters<S_in,S_out>& _cpm, const int _frame_size);
	virtual ~BCJR();

	// BCJR for the demodulation
	void decode(const mipp::vector<Q> &Lch_N,                                mipp::vector<Q> &Le_N);
	void decode(const mipp::vector<Q> &Lch_N, const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N);

private:
	void LLR_to_logsymb_proba    (const mipp::vector<Q> &Ldec_N                       );// retrieve log symbols probability from LLR
	void compute_alpha_beta_gamma(const mipp::vector<Q> &Lch_N                        );// compute gamma, alpha and beta, heart of the processing
	void symboles_probas         (                                                    );// from alpha, beta, and gamma computes new symbol probability
	void bits_probas             (                                                    );// from symbol probabilities, computes bit probabilities
	void compute_ext             (                               mipp::vector<Q> &Le_N);// extrinsic information processing from bit probabilities
	void compute_ext             (const mipp::vector<Q> &Ldec_N, mipp::vector<Q> &Le_N);// extrinsic information processing from bit probabilities and CPM a priori LLR
};

#include "BCJR.hxx"

#endif /* BCJR_HPP_ */
