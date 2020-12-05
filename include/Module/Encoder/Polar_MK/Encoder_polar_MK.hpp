/*!
 * \file
 * \brief Class module::Encoder_polar_MK.
 */
#ifndef ENCODER_POLAR_MK_HPP_
#define ENCODER_POLAR_MK_HPP_

#include <vector>

#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar_MK : public Encoder<B>, public tools::Interface_get_set_frozen_bits
{
protected:
	const tools::Polar_code&          code;
	      std::vector<bool>           frozen_bits;
	      std::vector<std::vector<B>> Ke;
	      std::vector<uint32_t>       idx;
	      std::vector<B>              u;

public:
	Encoder_polar_MK(const int& K, const int& N, const tools::Polar_code& code, const std::vector<bool>& frozen_bits);

	virtual ~Encoder_polar_MK() = default;

	virtual Encoder_polar_MK<B>* clone() const;

	// bool is_codeword(const B *X_N);
	virtual void set_frozen_bits(const std::vector<bool>& frozen_bits);
	virtual const std::vector<bool>& get_frozen_bits() const;

protected:
	virtual void _encode(const B *U_K, B *X_N, const size_t frame_id);
	void convert(const B *U_K, B *U_N);
	void light_encode(B *X_N);
};
}
}

#endif // ENCODER_POLAR_MK_HPP_
