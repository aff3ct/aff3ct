/*!
 * \file
 * \brief Class tools::User_pdf_noise_generator_fast.
 */
#ifndef User_pdf_noise_generator_fast_HPP_
#define User_pdf_noise_generator_fast_HPP_

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"
#include "Tools/Math/interpolation.h"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator_fast : public User_pdf_noise_generator<R>
{
private:
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

	R (*interp_function)(const R*, const R*, const unsigned, const R);

public:
	explicit User_pdf_noise_generator_fast(const tools::Distributions<R>& dists, const int seed = 0, Interpolation_type inter_type = Interpolation_type::NEAREST);
	virtual ~User_pdf_noise_generator_fast() = default;
	virtual User_pdf_noise_generator_fast<R>* clone() const;

	virtual void set_seed(const int seed);

	virtual void generate(                 R *draw, const unsigned length, const R noise_power);
	virtual void generate(const R* signal, R *draw, const unsigned length, const R noise_power);

private:
	inline mipp::Reg<R> get_random_simd();
	inline R            get_random     ();
};

template <typename R = float>
using User_pdf_gen_fast = User_pdf_noise_generator_fast<R>;
}
}

#endif /* User_pdf_noise_generator_fast_HPP_ */
