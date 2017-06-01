#ifdef CHANNEL_GSL

#ifndef NOISE_GSL_HPP_
#define NOISE_GSL_HPP_

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include "../Noise.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Noise_GSL : public Noise<R>
{
private:
	gsl_rng *rng;

public:
	Noise_GSL(const int seed = 0);
	virtual ~Noise_GSL();

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma);
};
}
}

#endif // NOISE_GSL_HPP_

#endif
