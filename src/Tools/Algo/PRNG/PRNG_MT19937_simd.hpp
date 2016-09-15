/*
This is an implementation of the fast pseudo-random number generator (PRNG) MT19937, colloquially called the Mersenne 
Twister. It was given this name because it has a period of 2^19937 - 1, which is a Mersenne prime.

The Mersenne Twister is highly regarded for its performance and high quality pseudo-random numbers. In spite of this, it 
is not suited for cryptographic code, because one only needs to observe 624 iterates to predict all future ones. It was 
designed with statistical simulations in mind, and should therefore be quite good for Monte Carlo simulations, 
probabilistic algorithms and so on.

This implementation is inspired by: https://github.com/cslarsen/mersenne-twister
*/

#ifndef PRNG_MT19937_SIMD_HPP
#define PRNG_MT19937_SIMD_HPP

#include <vector>

#include "Tools/Perf/MIPP/mipp.h"

class PRNG_MT19937_simd
{
protected:
	mipp::vector<mipp::Reg<int>> MT;
	unsigned index;

public:
	PRNG_MT19937_simd(const mipp::Reg<int> seed);
	PRNG_MT19937_simd();
	virtual ~PRNG_MT19937_simd();

	/*
	 * Initialize Mersenne Twister with given seed value.
	 */
	void seed(const mipp::Reg<int> seed);

	/*
	 * Extract a pseudo-random unsigned 32-bit integer in the range INT32_MIN ... INT32_MAX
	 */
	mipp::Reg<int> rand_s32();

	/*
	 * Return a random float in the CLOSED range [0, 1]
	 * Mnemonic: randf_co = random float 0=closed 1=closed
	 */
	mipp::Reg<float> randf_cc();

	/*
	 * Return a random float in the OPEN range [0, 1>
	 * Mnemonic: randf_co = random float 0=closed 1=open
	 */
	mipp::Reg<float> randf_co();

	/*
	 * Return a random float in the OPEN range <0, 1>
	 * Mnemonic: randf_oo = random float 0=open 1=open
	 */
	mipp::Reg<float> randf_oo();

private:
	void generate_numbers();
};

#endif // PRNG_MT19937_simd_FAST_HPP
