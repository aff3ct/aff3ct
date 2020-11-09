/*!
 * \file
 * \brief Class module::Encoder_polar_sys.
 */
#ifndef ENCODER_POLAR_SYS_HPP_
#define ENCODER_POLAR_SYS_HPP_

#include <vector>

#include "Module/Encoder/Polar/Encoder_polar.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar_sys : public Encoder_polar<B>
{
public:
	Encoder_polar_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits);
	virtual ~Encoder_polar_sys() = default;

	virtual Encoder_polar_sys<B>* clone() const;

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_POLAR_SYS_HPP_
