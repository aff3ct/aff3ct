#ifndef Decoder_chase_pyndiah_fast_HPP_
#define	Decoder_chase_pyndiah_fast_HPP_


#include "Decoder_chase_pyndiah.hpp"


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
	                           const int n_frames,
	                           Decoder_BCH<B,R> &dec,
	                           Encoder    <B  > &enc,
	                           const int n_least_reliable_positions = 2,
	                           const int n_test_vectors = 0,
	                           const int n_competitors  = 0,
	                           const std::vector<float>& cp_coef = {1,1,1,1,0}); // the a b c d and e coef

	virtual ~Decoder_chase_pyndiah_fast() = default;

protected:
	virtual void compute_metrics    (const R* Y_N);
	virtual void compute_reliability(const R* Y_N1, R* Y_N2);
};

}
}

#endif	/* Decoder_chase_pyndiah_fast_HPP_ */