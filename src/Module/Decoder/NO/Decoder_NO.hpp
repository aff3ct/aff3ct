#ifndef DECODER_NO_HPP_
#define DECODER_NO_HPP_

#include "../Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_NO : public Decoder_SISO<B,R>
{
public:
	Decoder_NO(const int K, const int n_frames = 1, const std::string name = "Decoder_NO");
	virtual ~Decoder_NO();

protected:
	void _hard_decode(const R *Y_K, B *V_K, const int frame_id);
	void _soft_decode(const R *sys, const R *par, R *ext, const int frame_id);
	void _soft_decode(const R *Y_K1, R *Y_K2, const int frame_id);
};
}
}

#endif /* DECODER_NO_HPP_ */
