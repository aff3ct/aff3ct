#ifdef CHANNEL_MKL

#ifndef NOISE_MKL
#define NOISE_MKL

#include <mkl_vsl.h>

#include "../Noise.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Noise_MKL : public Noise<R>
{
private:
	VSLStreamStatePtr* stream_state;

public:
	Channel_AWGN_MKL_LLR(const int seed = 0);
	virtual ~Channel_AWGN_MKL_LLR();

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma);
};
}
}

#endif // NOISE_MKL

#endif
