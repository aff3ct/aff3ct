#ifndef Decoder_chase_pyndiah_fast_HPP_
#define	Decoder_chase_pyndiah_fast_HPP_


#include "Decoder_chase_pyndiah.hpp"


namespace aff3ct
{
namespace module
{

/*
 * Chase :
 *   - take hard decision H on input R
 *   - create test vectors from test patterns after selecting the p least reliable positions -> give a p metric set Pm
 *   - hard decode with the HIHO decoder -> get the competitors C -> remove competitors that have not been corrected
 *   - compute the metrics Cm: euclidean distance of each competitor m compared to H
 *   - select the competitor with the smallest metric Dm -> get the decided word D
 *   - if SIHO, return D, if SISO, compute reliabilities of each bit of D -> Pyndiah
 * Pyndiah :
 *   - Compute extrinsic W as
 *          W = F - a * R
 *   - Compute the reliability F of D as, for each bit j of the word:
 *          Fj = Dj * [Cm - Dm] * b                   when Cj /= Dj in the competitor with the smallest metric Cm
 *             = Dj * [beta - c * Dm + d * |Rj|]   when there is no such competitor as described above
 *        with Dj =  1 when Hj = 0
 *                = -1 when Hj = 1
 *        with beta = sum from 0 to e of the Pm    where 0 <= e < p

 *   - a, b, c, d and e are simulation constants changeable by the user
 */

template <typename B = int, typename R = float>
class Decoder_chase_pyndiah_fast : public Decoder_chase_pyndiah<B,R>
{
	static_assert(mipp::nElReg<B>() == mipp::nElReg<R>(), "Type 'R' and 'B' have to be of the same size.");

public:
	Decoder_chase_pyndiah_fast(const int K, const int N, // N with the parity bit if any
	                           const int n_frames,
	                           Decoder_BCH<B,R> &dec,
	                           Encoder    <B  > &enc,
	                           const int n_least_reliable_positions = 2,
	                           const int n_test_vectors = 0,
	                           const int n_competitors  = 0,
	                           const std::vector<float>& cp_coef = {1,1,1,1,0}); // the a b c d and e coef

	~Decoder_chase_pyndiah_fast() = default;

protected:
	virtual void compute_metrics    (const R* Y_N);
	virtual void compute_reliability(const R* Y_N1, R* Y_N2);
};

}
}

#endif	/* Decoder_chase_pyndiah_fast_HPP_ */