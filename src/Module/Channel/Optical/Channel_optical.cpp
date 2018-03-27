#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Channel_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_optical<R>
::Channel_optical(const int N, tools::Noise_generator<R> *noise_generator_p0, tools::Noise_generator<R> *noise_generator_p1,
                  const R ROP, const int n_frames)
: Channel<R>(N, ROP, n_frames),
  noise_generator_p0(noise_generator_p0),
  noise_generator_p1(noise_generator_p1)
{
	const std::string name = "Channel_optical";
	this->set_name(name);

	if (noise_generator_p0 == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator_p0' can't be NULL.");

	if (noise_generator_p1 == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator_p1' can't be NULL.");
}

template <typename R>
Channel_optical<R>
::Channel_optical(const int N, const int seed, const R ROP, const int n_frames)
: Channel<R>(N, ROP, n_frames),
  noise_generator_p0(nullptr),//new tools::Gaussian_noise_generator_std<R>(seed))
  noise_generator_p1(nullptr)//new tools::Gaussian_noise_generator_std<R>(seed))
{
	const std::string name = "Channel_optical";
	this->set_name(name);
}

template <typename R>
Channel_optical<R>
::~Channel_optical()
{
	delete noise_generator_p0;
	delete noise_generator_p1;
}

template <typename R>
void Channel_optical<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	for (auto n = 0; n < this->N; n++)
		if (X_N[n])
			noise_generator_p1->generate(&this->noise[n], 1, this->sigma);
		else
			noise_generator_p0->generate(&this->noise[n], 1, this->sigma);

	std::copy(this->noise.begin(), this->noise.end(), Y_N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_optical<R_32>;
template class aff3ct::module::Channel_optical<R_64>;
#else
template class aff3ct::module::Channel_optical<R>;
#endif
// ==================================================================================== explicit template instantiation
