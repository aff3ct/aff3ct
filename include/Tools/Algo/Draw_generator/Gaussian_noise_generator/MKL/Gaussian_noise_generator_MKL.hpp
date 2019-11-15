/*!
 * \file
 * \brief Class tools::Gaussian_noise_generator_MKL.
 */
#ifdef AFF3CT_CHANNEL_MKL

#ifndef GAUSSIAN_NOISE_GENERATOR_MKL_HPP_
#define GAUSSIAN_NOISE_GENERATOR_MKL_HPP_

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Gaussian_noise_generator_MKL : public Gaussian_noise_generator<R>
{
private:
	void* stream_state; // VSLStreamStatePtr* type
	bool  is_stream_alloc;

public:
	explicit Gaussian_noise_generator_MKL(const int seed = 0);
	virtual ~Gaussian_noise_generator_MKL();
	virtual Gaussian_noise_generator_MKL<R>* clone() const;

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0);
};

template <typename R = float>
using Gaussian_gen_MKL = Gaussian_noise_generator_MKL<R>;
}
}

#endif // GAUSSIAN_NOISE_GENERATOR_MKL_HPP_

#endif
