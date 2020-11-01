/*!
 * \file
 * \brief Class module::Decoder_chase_pyndiah_fast.
 */
#ifndef Decoder_chase_pyndiah_fast_HPP_
#define	Decoder_chase_pyndiah_fast_HPP_

#include <vector>
#include <mipp.h>

#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int, typename R = float>
class Decoder_chase_pyndiah_fast : public Decoder_chase_pyndiah<B,R>
{
	static_assert(mipp::nElReg<B>() == mipp::nElReg<R>(), "Type 'R' and 'B' have to be of the same size.");

public:
	Decoder_chase_pyndiah_fast(const int K, const int N, // N with the parity bit if any
	                           const Decoder_BCH<B,R> &dec,
	                           const Encoder    <B  > &enc,
	                           const int n_least_reliable_positions = 2,
	                           const int n_test_vectors = 0,
	                           const int n_competitors  = 0,
	                           const std::vector<float>& cp_coef = {1,1,1,1,0}); // the a b c d and e coef

	virtual ~Decoder_chase_pyndiah_fast() = default;

	virtual Decoder_chase_pyndiah_fast<B,R>* clone() const;

protected:
	virtual void compute_metrics    (const R* Y_N);
	virtual void compute_reliability(const R* Y_N1, R* Y_N2);
};

}
}

#endif	/* Decoder_chase_pyndiah_fast_HPP_ */