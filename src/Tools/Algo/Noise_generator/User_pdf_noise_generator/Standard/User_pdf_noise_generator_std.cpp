#include <stdexcept>
#include <algorithm>

#include "User_pdf_noise_generator_std.hpp"

using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_std<R>
::User_pdf_noise_generator_std(const std::vector<R>& _xData, const std::vector<R>& _yData, const int seed)
: User_pdf_noise_generator<R>(_xData, _yData), uniform_dist(0, this->cdf.size()-1)
{
	this->set_seed(seed);
}

template <typename R>
User_pdf_noise_generator_std<R>
::User_pdf_noise_generator_std(const std::vector<Point<R>>& _pdf, const int seed)
: User_pdf_noise_generator<R>(_pdf), uniform_dist(0, this->cdf.size()-1)
{
	this->set_seed(seed);
}

template <typename R>
User_pdf_noise_generator_std<R>
::User_pdf_noise_generator_std(const std::vector<std::pair<R,R>>& _pdf, const int seed)
: User_pdf_noise_generator<R>(_pdf), uniform_dist(0, this->cdf.size()-1)
{
	this->set_seed(seed);
}

template <typename R>
User_pdf_noise_generator_std<R>
::~User_pdf_noise_generator_std()
{
}

template <typename R>
void User_pdf_noise_generator_std<R>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

template <typename R>
void User_pdf_noise_generator_std<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu)
{
	for (unsigned i = 0; i < length; i++)
		noise[i] = this->cdf[this->uniform_dist(this->rd_engine)].x();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::User_pdf_noise_generator_std<R_32>;
template class aff3ct::tools::User_pdf_noise_generator_std<R_64>;
#else
template class aff3ct::tools::User_pdf_noise_generator_std<R>;
#endif
// ==================================================================================== explicit template instantiation
