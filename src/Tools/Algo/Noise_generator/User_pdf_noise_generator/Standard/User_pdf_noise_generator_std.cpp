#include <algorithm>

#include "Tools/Math/interpolation.h"
#include "User_pdf_noise_generator_std.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_std<R>
::User_pdf_noise_generator_std(const tools::Distributions<R>& dists, const int seed)
: User_pdf_noise_generator<R>(dists), uniform_dist(0., 1.)
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
::generate(const R* signal, R *noise, const unsigned length, const R noise_power, const R mu)
{
	auto dis = this->distributions.get_distribution(noise_power);

	if (dis == nullptr)
	{
		std::stringstream message;
		message << "Undefined noise power 'noise_power' in the given distributions ('noise_power' = " << noise_power << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (unsigned i = 0; i < length; i++)
	{
		const auto& cdf_y = signal[i] ? dis->get_cdf_y()[1] : dis->get_cdf_y()[0];
		const auto& cdf_x = signal[i] ? dis->get_cdf_x()[1] : dis->get_cdf_x()[0];
		noise[i] = tools::linear_interpolation(cdf_y.data(), cdf_x.data(), cdf_x.size(),
		                                        this->uniform_dist(this->rd_engine));
	}
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
