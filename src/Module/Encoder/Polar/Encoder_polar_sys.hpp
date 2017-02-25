#ifndef ENCODER_POLAR_SYS_HPP_
#define ENCODER_POLAR_SYS_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Encoder_polar.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Encoder_polar_sys : public Encoder_polar<B>
{
public:
	Encoder_polar_sys(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int& n_frames = 1,
	                  const std::string name = "Encoder_polar_sys");
	virtual ~Encoder_polar_sys() {}

	void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};
}
}

#endif // ENCODER_POLAR_SYS_HPP_
