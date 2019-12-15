#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/Standard/User_pdf_noise_generator_std.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_std<R>
::User_pdf_noise_generator_std(const tools::Distributions<R>& dists, const int seed, Interpolation_type inter_type)
: User_pdf_noise_generator<R>(dists), uniform_dist(0., 1.), interp_function(nullptr)
{
	this->set_seed(seed);

	switch (inter_type)
	{
		case Interpolation_type::LINEAR:
			interp_function = tools::linear_interpolation<R>;
			break;

		case Interpolation_type::NEAREST:
			interp_function = tools::nearest_interpolation<R>;
			break;
	}
}

template <typename R>
User_pdf_noise_generator_std<R>* User_pdf_noise_generator_std<R>
::clone() const
{
	return new User_pdf_noise_generator_std(*this);
}

template <typename R>
void User_pdf_noise_generator_std<R>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

template <typename R>
void User_pdf_noise_generator_std<R>
::generate(const R* signal, R *draw, const unsigned length, const R noise_power)
{
	auto dis = this->distributions.get_distribution(noise_power);

	for (unsigned i = 0; i < length; i++)
	{
		const auto& cdf_y = signal[i] ? dis.get_cdf_y()[1] : dis.get_cdf_y()[0];
		const auto& cdf_x = signal[i] ? dis.get_cdf_x()[1] : dis.get_cdf_x()[0];
		draw[i] = interp_function(cdf_y.data(), cdf_x.data(), (const unsigned)cdf_x.size(), this->uniform_dist(this->rd_engine));
	}
}

template <typename R>
void User_pdf_noise_generator_std<R>
::generate(R *draw, const unsigned length, const R noise_power)
{
	throw unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::User_pdf_noise_generator_std<R_32>;
template class aff3ct::tools::User_pdf_noise_generator_std<R_64>;
#else
template class aff3ct::tools::User_pdf_noise_generator_std<R>;
#endif
// ==================================================================================== explicit template instantiation
