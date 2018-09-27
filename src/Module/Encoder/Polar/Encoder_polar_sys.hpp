#ifndef ENCODER_POLAR_SYS_HPP_
#define ENCODER_POLAR_SYS_HPP_

#include <vector>

#include "Encoder_polar.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar_sys : public Encoder_polar<B>
{
public:
	Encoder_polar_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits, const int& n_frames = 1);
	virtual ~Encoder_polar_sys() = default;

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_POLAR_SYS_HPP_
