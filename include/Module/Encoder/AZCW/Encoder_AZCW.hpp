/*!
 * \file
 * \brief Class module::Encoder_AZCW.
 */
#ifndef ENCODER_AZCW_HPP_
#define ENCODER_AZCW_HPP_

#include <cstdint>
#include <vector>

#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_AZCW : public Encoder<B>
{
public:
	Encoder_AZCW(const int K, const int N);
	virtual ~Encoder_AZCW() = default;

	virtual Encoder_AZCW<B>* clone() const;

	const std::vector<uint32_t>& get_info_bits_pos() const;

	bool is_sys() const;

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
};
}
}

#endif /* ENCODER_AZCW_HPP_ */
