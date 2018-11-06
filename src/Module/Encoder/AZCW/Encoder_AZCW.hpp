#ifndef ENCODER_AZCW_HPP_
#define ENCODER_AZCW_HPP_

#include <random>

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_AZCW : public Encoder<B>
{
public:
	Encoder_AZCW(const int K, const int N, const int n_frames = 1);
	virtual ~Encoder_AZCW() = default;

	const std::vector<uint32_t>& get_info_bits_pos() const;

	bool is_sys() const;

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif /* ENCODER_AZCW_HPP_ */
