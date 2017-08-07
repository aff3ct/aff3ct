#include <stdexcept>
#include <algorithm>

#include "Noise_std.hpp"

using namespace aff3ct::tools;

template <typename R>
Noise_std<R>
::Noise_std(const int seed)
: Noise<R>()
{
	this->set_seed(seed);
}

template <typename R>
Noise_std<R>
::~Noise_std()
{
}

template <typename R>
void Noise_std<R>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

template <typename R>
void Noise_std<R>
::generate(R *noise, const unsigned length, const R sigma)
{
	normal_dist = std::normal_distribution<R>((R)0, sigma);

	for (unsigned i = 0; i < length; i++)
		noise[i] = this->normal_dist(this->rd_engine);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Noise_std<R_32>;
template class aff3ct::tools::Noise_std<R_64>;
#else
template class aff3ct::tools::Noise_std<R>;
#endif
// ==================================================================================== explicit template instantiation
