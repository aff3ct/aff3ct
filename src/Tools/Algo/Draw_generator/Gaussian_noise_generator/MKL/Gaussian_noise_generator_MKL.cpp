#ifdef AFF3CT_CHANNEL_MKL

#include <mkl_vsl.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/MKL/Gaussian_noise_generator_MKL.hpp"

using namespace aff3ct::tools;

template <typename R>
Gaussian_noise_generator_MKL<R>
::Gaussian_noise_generator_MKL(const int seed)
: Gaussian_noise_generator<R>(), stream_state((void*)new VSLStreamStatePtr), is_stream_alloc(false)
{
	this->set_seed(seed);
}

template <typename R>
Gaussian_noise_generator_MKL<R>
::~Gaussian_noise_generator_MKL()
{
	if (is_stream_alloc)
		vslDeleteStream((VSLStreamStatePtr*)stream_state);
	delete (VSLStreamStatePtr*)stream_state;
}

template <typename R>
Gaussian_noise_generator_MKL<R>* Gaussian_noise_generator_MKL<R>
::clone() const
{
	Gaussian_noise_generator_MKL<R>* eg = new Gaussian_noise_generator_MKL(*this);
	VSLStreamStatePtr* new_ptr = new VSLStreamStatePtr;
	vslCopyStream(new_ptr, *(VSLStreamStatePtr*)this->stream_state);
	eg->stream_state = (void*)new_ptr;
	return eg;
}

template <typename R>
void Gaussian_noise_generator_MKL<R>
::set_seed(const int seed)
{
	if (is_stream_alloc) vslDeleteStream((VSLStreamStatePtr*)stream_state);

	//vslNewStream((VSLStreamStatePtr*)stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream((VSLStreamStatePtr*)stream_state, VSL_BRNG_SFMT19937, seed);

	is_stream_alloc = true;
}

template <typename R>
void Gaussian_noise_generator_MKL<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "Adding white Gaussian noise is impossible on this data type.");
}

namespace aff3ct
{
namespace tools
{
template <>
void Gaussian_noise_generator_MKL<float>
::generate(float *noise, const unsigned length, const float sigma, const float mu)
{
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              *(VSLStreamStatePtr*)stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	/*
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              *(VSLStreamStatePtr*)stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	*/
}
}
}

namespace aff3ct
{
namespace tools
{
template <>
void Gaussian_noise_generator_MKL<double>
::generate(double *noise, const unsigned length, const double sigma, const double mu)
{
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              *(VSLStreamStatePtr*)stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	/*
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              *(VSLStreamStatePtr*)stream_state,
	              length,
	              noise,
	              mu,
	              sigma);
	*/
}
}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Gaussian_noise_generator_MKL<R_32>;
template class aff3ct::tools::Gaussian_noise_generator_MKL<R_64>;
#else
template class aff3ct::tools::Gaussian_noise_generator_MKL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
