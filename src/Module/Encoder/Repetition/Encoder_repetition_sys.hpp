#ifndef ENCODER_REPETITION_SYS_HPP_
#define ENCODER_REPETITION_SYS_HPP_

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_repetition_sys : public Encoder<B>
{
protected:
	const int rep_count; // number of repetition

	const bool buffered_encoding;

public:
	Encoder_repetition_sys(const int& K, const int& N, const bool buffered_encoding = true, const int n_frames = 1);
	virtual ~Encoder_repetition_sys() = default;

	bool is_codeword(const B *X_N);

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_REPETITION_SYS_HPP_
