#ifndef ENCODER_BCH_HPP_
#define ENCODER_BCH_HPP_

#include <vector>

#include "../Encoder.hpp"
#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_BCH : public Encoder<B>
{

protected:
	const std::vector<int>& g;  // coefficients of the generator polynomial, g(x)
	std::vector<B>          bb; // coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)

public:
	Encoder_BCH(const int& K, const int& N, const tools::BCH_polynomial_generator& GF, const int n_frames = 1);

	virtual ~Encoder_BCH();

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_BCH_HPP_
