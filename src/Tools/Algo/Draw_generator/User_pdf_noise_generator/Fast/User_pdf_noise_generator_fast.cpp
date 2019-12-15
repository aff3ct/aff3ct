#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/Fast/User_pdf_noise_generator_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
User_pdf_noise_generator_fast<R>
::User_pdf_noise_generator_fast(const tools::Distributions<R>& dists, const int seed, Interpolation_type inter_type)
: User_pdf_noise_generator<R>(dists), interp_function(nullptr)
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
User_pdf_noise_generator_fast<R>* User_pdf_noise_generator_fast<R>
::clone() const
{
	return new User_pdf_noise_generator_fast(*this);
}

template <typename R>
void User_pdf_noise_generator_fast<R>
::set_seed(const int seed)
{
	mt19937.seed(seed);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename R>
mipp::Reg<R> User_pdf_noise_generator_fast<R>
::get_random_simd()
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

template <typename R>
R User_pdf_noise_generator_fast<R>
::get_random()
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

namespace aff3ct
{
namespace tools
{
template <>
mipp::Reg<float> User_pdf_noise_generator_fast<float>
::get_random_simd()
{
	// return a vector of numbers between [0,1]
	return mt19937_simd.randf_cc();
}
}
}

namespace aff3ct
{
namespace tools
{
template <>
float User_pdf_noise_generator_fast<float>
::get_random()
{
	// return a number between  [0,1]
	return mt19937.randf_cc();
}
}
}

template <typename R>
void User_pdf_noise_generator_fast<R>
::generate(const R* signal, R *draw, const unsigned length, const R noise_power)
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

namespace aff3ct
{
namespace tools
{
template <>
void User_pdf_noise_generator_fast<float>
::generate(const float* signal, float *draw, const unsigned length, const float noise_power)
{
	auto dis = this->distributions.get_distribution(noise_power);

	const unsigned vec_loop_size = (length / mipp::N<float>()) * mipp::N<float>();

	for (unsigned i = 0; i < vec_loop_size; i += mipp::N<float>())
	{
		const auto r_draw  = get_random_simd();
		r_draw.store(draw + i);
	}

	for (auto i = vec_loop_size; i < length; i++)
		draw[i] = get_random();


	for (unsigned i = 0; i < length; i++)
	{
		const auto& cdf_y = signal[i] ? dis.get_cdf_y()[1] : dis.get_cdf_y()[0];
		const auto& cdf_x = signal[i] ? dis.get_cdf_x()[1] : dis.get_cdf_x()[0];
		draw[i] = interp_function(cdf_y.data(), cdf_x.data(), (unsigned)cdf_x.size(), draw[i]);
	}
}
}
}

template <typename R>
void User_pdf_noise_generator_fast<R>
::generate(R *draw, const unsigned length, const R noise_power)
{
	throw unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::User_pdf_noise_generator_fast<R_32>;
template class aff3ct::tools::User_pdf_noise_generator_fast<R_64>;
#else
template class aff3ct::tools::User_pdf_noise_generator_fast<R>;
#endif
// ==================================================================================== explicit template instantiation
