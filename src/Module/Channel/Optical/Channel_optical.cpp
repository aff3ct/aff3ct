#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Channel_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_optical<R>
::Channel_optical(const int N, tools::Noise_generator<R> *noise_generator,
                  const R sigma, const int n_frames)
: Channel<R>(N, 1, n_frames),
  noise_generator(noise_generator)
{
	const std::string name = "Channel_optical";
	this->set_name(name);

	if (noise_generator == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator' can't be NULL.");

	this->ebn0 = sigma;
}

template <typename R>
Channel_optical<R>
::~Channel_optical()
{
	if (noise_generator != nullptr) delete noise_generator;
}

template <typename R>
void Channel_optical<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	noise_generator->generate(X_N, Y_N, this->N, this->ebn0);
}

template <typename R>
void Channel_optical<R>
::set_noise(const R sigma, const R esn0, const R ebn0)
{
	this->sigma = sigma;
	this->esn0  = esn0;
	this->ebn0  = ebn0;
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
