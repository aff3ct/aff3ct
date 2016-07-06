/*
This is an implementation of the fast pseudo-random number generator (PRNG) MT19937, colloquially called the Mersenne 
Twister. It was given this name because it has a period of 2^19937 - 1, which is a Mersenne prime.

The Mersenne Twister is highly regarded for its performance and high quality pseudo-random numbers. In spite of this, it 
is not suited for cryptographic code, because one only needs to observe 624 iterates to predict all future ones. It was 
designed with statistical simulations in mind, and should therefore be quite good for Monte Carlo simulations, 
probabilistic algorithms and so on.

This implementation is inspired by: https://github.com/cslarsen/mersenne-twister
*/

#ifndef PRNG_MT19937_FAST_HPP
#define PRNG_MT19937_FAST_HPP

#define __STDC_LIMIT_MACROS
#include <cstdint>
#include <vector>

#include "../MIPP/mipp.h"

class PRNG_MT19937_fast
{
protected:
	mipp::vector<mipp::Reg<int>> MT;
	unsigned index;

public:
	PRNG_MT19937_fast(const mipp::Reg<int> seed);
	virtual ~PRNG_MT19937_fast();

	/*
	 * Extract a pseudo-random integer in the range 0 ... RAND_MAX.
	 * (LIBC REPLACEMENT FUNCTION)
	 */
	mipp::Reg<int> rand();

	/*
	 * Extract a pseudo-random unsigned 32-bit integer in the range 0 ... UINT32_MAX
	 */
	mipp::Reg<int> rand_u32();

	/*
	 * Combine two unsigned 32-bit pseudo-random numbers into one 64-bit
	 */
	// mipp::Reg<long long> rand_u64();

	/*
	 * Return a random float in the CLOSED range [0, 1]
	 * Mnemonic: randf_co = random float 0=closed 1=closed
	 */
	// mipp::Reg<float> randf_cc();

	/*
	 * Return a random float in the OPEN range [0, 1>
	 * Mnemonic: randf_co = random float 0=closed 1=open
	 */
	// mipp::Reg<float> randf_co();

	/*
	 * Return a random float in the OPEN range <0, 1>
	 * Mnemonic: randf_oo = random float 0=open 1=open
	 */
	mipp::Reg<float> randf_oo();

	/*
	 * Return a random double in the CLOSED range [0, 1]
	 * Mnemonic: randd_co = random double 0=closed 1=closed
	 */
	// mipp::Reg<double> randd_cc();

	/*
	 * Return a random double in the OPEN range [0, 1>
	 * Mnemonic: randd_co = random double 0=closed 1=open
	 */
	// mipp::Reg<double> randd_co();

	/*
	 * Return a random double in the OPEN range <0, 1>
	 * Mnemonic: randd_oo = random double 0=open 1=open
	 */
	// mipp::Reg<double> randd_oo();

private:
	void generate_numbers();
};

#endif // PRNG_MT19937_fast_FAST_HPP