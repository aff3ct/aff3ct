/*!
 * \file
 * \brief Class tools::CPM_BCJR.
 */
#ifndef CPM_BCJR_HPP_
#define CPM_BCJR_HPP_

#include <vector>

#include "Tools/Math/max.h"
#include "Tools/Code/CPM/CPM_parameters.hpp"

namespace aff3ct
{
namespace tools
{
template <typename SIN = int, typename SOUT = int, typename Q = float, proto_max<Q> MAX = max_star>
class CPM_BCJR
{
protected:
	const CPM_parameters<SIN,SOUT>& cpm; // all CPM parameters
	const int n_symbols;                 // size of a frame (in symbols) from the channel (with tail bits)
	const int chn_size;                  // size of a frame (wave form probas) from the channel (with tail bits)
	const int dec_size;                  // size of a frame (bits proba) from the decoder
	const int ext_size;                  // size of a frame (bits proba) from the bcjr

	std::vector<Q> symb_apriori_prob;
	std::vector<Q> gamma;
	std::vector<Q> alpha;
	std::vector<Q> beta;
	std::vector<Q> proba_msg_symb;
	std::vector<Q> proba_msg_bits;

public:
	CPM_BCJR(const CPM_parameters<SIN,SOUT>& _cpm, const int _frame_size);
	virtual ~CPM_BCJR() = default;

	// CPM_BCJR for the demodulation
	void decode(const std::vector<Q> &Lch_N,                               std::vector<Q> &Le_N);
	void decode(const std::vector<Q> &Lch_N, const std::vector<Q> &Ldec_N, std::vector<Q> &Le_N);
	void decode(const Q *Lch_N,                  Q *Le_N);
	void decode(const Q *Lch_N, const Q *Ldec_N, Q *Le_N);

private:
	void LLR_to_logsymb_proba    (const Q *Ldec_N); // retrieve log symbols probability from LLR
	void compute_alpha_beta_gamma(const Q *Lch_N ); // compute gamma, alpha and beta, heart of the processing
	void symboles_probas         (               ); // from alpha, beta, and gamma computes new symbol probability
	void bits_probas             (               ); // from symbol probabilities, computes bit probabilities
	void compute_ext             (      Q *Le_N  ); // extrinsic information processing from bit probabilities
	void compute_ext             (const Q *Ldec_N,
	                                    Q *Le_N  ); // extrinsic information processing from bit probabilities
	                                                // and CPM a priori LLR
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/CPM/BCJR/CPM_BCJR.hxx"
#endif

#endif /* CPM_BCJR_HPP_ */
