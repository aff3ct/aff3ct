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
	Decoder_NO(const int K, const int N, const int n_frames = 1, const std::string name = "Decoder_NO");
	virtual ~Decoder_NO();

protected:
	void _hard_decode(const R *Y_N, B *V_K);
	void _soft_decode(const R *sys, const R *par, R *ext);
	void _soft_decode(const R *Y_N1, R *Y_N2);
};
}
}

#endif /* DECODER_NO_HPP_ */
