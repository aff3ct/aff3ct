/*!
 * \file
 * \brief The Mersenne Twister pseudo-random number generator (PRNG).
 *
 * This is an implementation of fast PRNG called MT19937,
 * meaning it has a period of 2^19937-1, which is a Mersenne
 * prime.
 *
 * This PRNG is fast and suitable for non-cryptographic code.
 * For instance, it would be perfect for Monte Carlo simulations,
 * etc.
 *
 * This code has been designed as a drop-in replacement for libc rand and
 * srand().
 *
 * This file is under BSD license.
 *
 * \author Christian Stigen Larsen
 * \author Adrien Cassagne
 *
 * \date 2015-02-17
 *
 */

#ifndef PRNG_MT19937_HPP
#define PRNG_MT19937_HPP

#define __STDC_LIMIT_MACROS
#include <cstdint>
#include <vector>

namespace aff3ct
{
namespace tools
{
/*!
 * \class PRNG_MT19937
 * \brief The Mersenne Twister pseudo-random number generator (PRNG).
 */
class PRNG_MT19937
{
protected:
	std::vector<uint32_t> MT;
	unsigned index;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param seed: a seed to initialize the PRNG.
	 */
	explicit PRNG_MT19937(const uint32_t seed = 0);

	/*!
	 * \brief Destructor.
	 */
	virtual ~PRNG_MT19937();

	/*!
	 * \brief Initialize the PRNG with given seed value.
	 *
	 * \param seed: a seed.
	 */
	void seed(const uint32_t seed);

	/*!
	 * \brief Extract a pseudo-random integer in the range 0 ... RAND_MAX. (LIBC REPLACEMENT FUNCTION).
	 *
	 * \return a pseudo random number.
	 */
	int rand();

	/*!
	 * \brief Extract a pseudo-random unsigned 32-bit integer in the range 0 ... UINT32_MAX.
	 *
	 * \return a pseudo random number.
	 */
	uint32_t rand_u32();

	/*!
	 * \brief Combine two unsigned 32-bit pseudo-random numbers into one 64-bit.
	 *
	 * \return a pseudo random number.
	 */
	uint64_t rand_u64();

	/*!
	 * \brief Return a random float in the CLOSED range [0, 1]
	 * Mnemonic: randf_co = random float 0=closed 1=closed.
	 *
	 * \return a pseudo random number.
	 */
	float randf_cc();

	/*!
	 * \brief Return a random float in the OPEN range [0, 1>
	 * Mnemonic: randf_co = random float 0=closed 1=open.
	 *
	 * \return a pseudo random number.
	 */
	float randf_co();

	/*!
	 * \brief Return a random float in the OPEN range <0, 1>
	 * Mnemonic: randf_oo = random float 0=open 1=open.
	 *
	 * \return a pseudo random number.
	 */
	float randf_oo();

	/*!
	 * \brief Return a random double in the CLOSED range [0, 1]
	 * Mnemonic: randd_co = random double 0=closed 1=closed.
	 *
	 * \return a pseudo random number.
	 */
	double randd_cc();

	/*!
	 * \brief Return a random double in the OPEN range [0, 1>
	 * Mnemonic: randd_co = random double 0=closed 1=open.
	 *
	 * \return a pseudo random number.
	 */
	double randd_co();

	/*!
	 * \brief Return a random double in the OPEN range <0, 1>
	 * Mnemonic: randd_oo = random double 0=open 1=open.
	 *
	 * \return a pseudo random number.
	 */
	double randd_oo();

private:
	void generate_numbers();
};
}
}

#endif // PRNG_MT19937_HPP
