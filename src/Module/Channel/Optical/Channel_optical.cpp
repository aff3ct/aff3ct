#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common/hamming_distance.h"

#include "Channel_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_optical<R>
::Channel_optical(const int N, tools::Noise_generator<R> *noise_generator_p0, tools::Noise_generator<R> *noise_generator_p1,
                  const R ROP, const int n_frames)
: Channel<R>(N, ROP, n_frames),
  noise_generator_p0(noise_generator_p0),
  noise_generator_p1(noise_generator_p1),
  gene_noise0(this->N),
  gene_noise1(this->N)
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
::~Channel_optical()
{
	if (noise_generator_p0 != nullptr) delete noise_generator_p0;
	if (noise_generator_p1 != nullptr) delete noise_generator_p1;
}

template <typename R>
void Channel_optical<R>
::set_sigma(const R ROP)
{
	this->sigma = ROP;
}


template <typename R>
void Channel_optical<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	auto n1 = tools::hamming_distance(X_N, this->N); // number of 1 in the frame

	noise_generator_p1->generate(this->gene_noise1.data(), n1, this->sigma);
	noise_generator_p0->generate(this->gene_noise0.data(), this->N - n1, this->sigma);

	unsigned idx0 = 0, idx1 = 0;
	for (auto n = 0; n < this->N; n++)
	{
		if (X_N[n])
			this->noise[frame_id * this->N + n] = this->gene_noise1[idx1++];
		else
			this->noise[frame_id * this->N + n] = this->gene_noise0[idx0++];

		Y_N[n] = this->noise[frame_id * this->N + n];
	}
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
