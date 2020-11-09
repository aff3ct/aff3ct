/*!
 * \file
 * \brief Class module::Encoder_polar_MK_sys.
 */
#ifndef ENCODER_POLAR_MK_SYS_HPP_
#define ENCODER_POLAR_MK_SYS_HPP_

#include <vector>

#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar_MK_sys : public Encoder_polar_MK<B>
{
public:
	Encoder_polar_MK_sys(const int& K, const int& N, const tools::Polar_code& code,
	                     const std::vector<bool>& frozen_bits);

	virtual ~Encoder_polar_MK_sys() = default;

	virtual Encoder_polar_MK_sys<B>* clone() const;

protected:
	virtual void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif // ENCODER_POLAR_MK_SYS_HPP_
