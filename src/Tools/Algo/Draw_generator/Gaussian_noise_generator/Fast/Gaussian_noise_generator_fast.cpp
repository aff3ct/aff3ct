#include <cmath>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Fast/Gaussian_noise_generator_fast.hpp"

using namespace aff3ct::tools;

template <typename R>
Gaussian_noise_generator_fast<R>
::Gaussian_noise_generator_fast(const int seed)
: Gaussian_noise_generator<R>(),
  mt19937(),
  mt19937_simd()
{
	this->set_seed(seed);
}

template <typename R>
Gaussian_noise_generator_fast<R>* Gaussian_noise_generator_fast<R>
::clone() const
{
	return new Gaussian_noise_generator_fast(*this);
}

template <typename R>
void Gaussian_noise_generator_fast<R>
::set_seed(const int seed)
{
	mt19937.seed(seed);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename R>
mipp::Reg<R> Gaussian_noise_generator_fast<R>
::get_random_simd()
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

template <typename R>
R Gaussian_noise_generator_fast<R>
::get_random()
{
	throw runtime_error(__FILE__, __LINE__, __func__, "The MT19937 random generator does not support this type.");
}

namespace aff3ct
{
namespace tools
{
template <>
mipp::Reg<float> Gaussian_noise_generator_fast<float>
::get_random_simd()
{
	// return a vector of numbers between ]0,1[
	return mt19937_simd.randf_oo();
}
}
}

namespace aff3ct
{
namespace tools
{
template <>
float Gaussian_noise_generator_fast<float>
::get_random()
{
	// return a number between ]0,1[
	return mt19937.randf_oo();
}
}
}

template <typename R>
void Gaussian_noise_generator_fast<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu) //TODO: integrate mu in the computation
{
	if (!mipp::isAligned(noise))
		throw runtime_error(__FILE__, __LINE__, __func__, "'noise' is misaligned memory.");

	const auto twopi = (R)(2.0 * 3.14159265358979323846);

	// SIMD version of the Box Muller method in the polar form
	const auto vec_loop_size = (int)(((int)length / (mipp::nElReg<R>() * 2)) * mipp::nElReg<R>() * 2);
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>() * 2)
	{
		const auto u1 = get_random_simd();
		const auto u2 = get_random_simd();

		const auto radius = mipp::sqrt(mipp::log(u1) * (R)-2.0) * sigma;
		const auto theta  = u2 * twopi;

		mipp::Reg<R> sintheta, costheta;
		mipp::sincos(theta, sintheta, costheta);

		auto awgn1 = radius * costheta + mu;
		auto awgn2 = radius * sintheta + mu;

		awgn1.store(&noise[i                    ]);
		awgn2.store(&noise[i + mipp::nElReg<R>()]);
	}

	// seq version of the Box Muller method in the polar form
	const auto seq_loop_size = (int)(length / 2) * 2;
	for (auto i = vec_loop_size; i < seq_loop_size; i += 2)
	{
		const auto u1 = get_random();
		const auto u2 = get_random();

		const auto radius = (R)std::sqrt(std::log(u1) * (R)-2.0) * sigma;
		const auto theta  = u2 * twopi;

		const auto sintheta = std::sin(theta);
		const auto costheta = std::cos(theta);

		noise[i +0] = radius * sintheta + mu;
		noise[i +1] = radius * costheta + mu;
	}

	// distribute the last odd element
	if ((int)length != seq_loop_size)
	{
		const auto u1 = get_random();
		const auto u2 = get_random();

		const auto radius = (R)std::sqrt(std::log(u1) * (R)-2.0) * sigma;
		const auto theta  = twopi * u2;

		const auto sintheta = std::sin(theta);

		noise[length -1] = radius * sintheta + mu;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Gaussian_noise_generator_fast<R_32>;
template class aff3ct::tools::Gaussian_noise_generator_fast<R_64>;
#else
template class aff3ct::tools::Gaussian_noise_generator_fast<R>;
#endif
// ==================================================================================== explicit template instantiation
