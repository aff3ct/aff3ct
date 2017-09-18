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
	VSLStreamStatePtr stream_state;
	bool              is_stream_alloc;

public:
	Noise_MKL(const int seed = 0);
	virtual ~Noise_MKL();

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0);
};
}
}

#endif // NOISE_MKL

#endif
