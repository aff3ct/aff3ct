#ifndef ENCODER_NO_HPP_
#define ENCODER_NO_HPP_

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_NO : public Encoder<B>
{
public:
	Encoder_NO(const int K, const int n_frames = 1);
	virtual ~Encoder_NO() = default;

	bool is_codeword(const B *X_K);

protected:
	void _encode(const B *U_K, B *X_K, const int frame_id);
};
}
}

#endif /* ENCODER_NO_HPP_ */
