#ifndef ENCODER_BCH_ADAPTATIVE_HPP_
#define ENCODER_BCH_ADAPTATIVE_HPP_

#include <vector>

#include "../Encoder_BCH.hpp"
#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_BCH_adaptative : public Encoder_BCH<B>
{
protected:
	const int K_adap;
	const int N_adap;

public:
	Encoder_BCH_adaptative(const int& K, const int& N, const tools::BCH_polynomial_generator& GF, const int n_frames = 1);

	virtual ~Encoder_BCH_adaptative();

protected:
	virtual void  _encode(const B *U_K, B *X_N, const int frame_id);
	virtual void __encode(const B *U_K, B *bb);
};
}
}

#endif // ENCODER_BCH_ADAPTATIVE_HPP_
