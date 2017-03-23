#ifndef ENCODER_BCH_HPP_
#define ENCODER_BCH_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder.hpp"
#include "Tools/Math/Galois.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Encoder_BCH : public Encoder<B>
{

protected:
	const int m; // order of the Galois Field

	mipp::vector<int> g;  // coefficients of the generator polynomial, g(x)
	mipp::vector<int> bb; // coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)

public:
	Encoder_BCH(const int& K, const int& N, const int& m, const tools::Galois &GF,
	            const int n_frames = 1, const std::string name = "Encoder_BCH");

	virtual ~Encoder_BCH() {}

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);

private:
	void _encode(const B* U_K, B* X_N);
};
}
}

#endif // ENCODER_BCH_HPP_
