/*!
 * \file
 * \brief The Mersenne Twister pseudo-random number generator (PRNG) with fast SIMD instructions.
 *
 * This is an implementation of fast PRNG called MT19937,
 * meaning it has a period of 2^19937-1, which is a Mersenne
 * prime.
 *
 * This PRNG is fast and suitable for non-cryptographic code.
 * For instance, it would be perfect for Monte Carlo simulations,
 * etc.
 *
 * This implementation is inspired by: https://github.com/cslarsen/mersenne-twister.
 * It takes advantage of the fast SIMD instructions of the modern CPUs.
 *
 */

#ifndef PRNG_MT19937_SIMD_HPP
#define PRNG_MT19937_SIMD_HPP

#include <cstdint>
#include <mipp.h>

namespace aff3ct
{
namespace tools
{
/*!
 * \class PRNG_MT19937_simd
 * \brief The Mersenne Twister pseudo-random number generator (PRNG) with fast SIMD instructions.
 */
class PRNG_MT19937_simd
{
protected:
	mipp::vector<mipp::Reg<int32_t>> MT;
	unsigned index;

public:
	explicit PRNG_MT19937_simd(const mipp::Reg<int32_t> seed);
	PRNG_MT19937_simd();
	virtual ~PRNG_MT19937_simd();

	/*!
	 * \brief Initializes Mersenne Twister with given seed value.
	 *
	 * \param seed: a vector register of seeds to initialize the PRNG.
	 */
	void seed(const mipp::Reg<int32_t> seed);

	/*!
	 * \brief Extracts a pseudo-random signed 32-bit integer in the range INT32_MIN ... INT32_MAX.
	 *
	 * \return a vector register of pseudo random numbers.
	 */
	mipp::Reg<int32_t> rand_s32();

	/*!
	 * \brief Returns a random float in the CLOSED range [0, 1]
	 * Mnemonic: randf_co = random float 0=closed 1=closed.
	 *
	 * \return a vector register of pseudo random numbers.
	 */
	mipp::Reg<float> randf_cc();

	/*!
	 * \brief Returns a random float in the OPEN range [0, 1>
	 * Mnemonic: randf_co = random float 0=closed 1=open.
	 *
	 * \return a vector register of pseudo random numbers.
	 */
	mipp::Reg<float> randf_co();

	/*!
	 * \brief Returns a random float in the OPEN range <0, 1>
	 * Mnemonic: randf_oo = random float 0=open 1=open.
	 *
	 * \return a vector register of pseudo random numbers.
	 */
	mipp::Reg<float> randf_oo();

private:
	void generate_numbers();
};
}
}

#endif // PRNG_MT19937_simd_FAST_HPP
