/*
This is an implementation of the fast pseudo-random number generator (PRNG) MT19937, colloquially called the Mersenne 
Twister. It was given this name because it has a period of 2^19937 - 1, which is a Mersenne prime.

The Mersenne Twister is highly regarded for its performance and high quality pseudo-random numbers. In spite of this, it 
is not suited for cryptographic code, because one only needs to observe 624 iterates to predict all future ones. It was 
designed with statistical simulations in mind, and should therefore be quite good for Monte Carlo simulations, 
probabilistic algorithms and so on.

This implementation is inspired by: https://github.com/cslarsen/mersenne-twister
*/

#ifndef PRNG_MT19937_HPP
#define PRNG_MT19937_HPP

#define __STDC_LIMIT_MACROS
#include <cstdint>
#include <vector>

class PRNG_MT19937
{
protected:
	std::vector<uint32_t> MT;
	unsigned index;

public:
	PRNG_MT19937(const uint32_t seed = 0);
	virtual ~PRNG_MT19937();

	/*
	 * Extract a pseudo-random integer in the range 0 ... RAND_MAX.
	 * (LIBC REPLACEMENT FUNCTION)
	 */
	int rand();

	/*
	 * Extract a pseudo-random unsigned 32-bit integer in the range 0 ... UINT32_MAX
	 */
	uint32_t rand_u32();

	/*
	 * Combine two unsigned 32-bit pseudo-random numbers into one 64-bit
	 */
	uint64_t rand_u64();

	/*
	 * Return a random float in the CLOSED range [0, 1]
	 * Mnemonic: randf_co = random float 0=closed 1=closed
	 */
	float randf_cc();

	/*
	 * Return a random float in the OPEN range [0, 1>
	 * Mnemonic: randf_co = random float 0=closed 1=open
	 */
	float randf_co();

	/*
	 * Return a random float in the OPEN range <0, 1>
	 * Mnemonic: randf_oo = random float 0=open 1=open
	 */
	float randf_oo();

	/*
	 * Return a random double in the CLOSED range [0, 1]
	 * Mnemonic: randd_co = random double 0=closed 1=closed
	 */
	double randd_cc();

	/*
	 * Return a random double in the OPEN range [0, 1>
	 * Mnemonic: randd_co = random double 0=closed 1=open
	 */
	double randd_co();

	/*
	 * Return a random double in the OPEN range <0, 1>
	 * Mnemonic: randd_oo = random double 0=open 1=open
	 */
	double randd_oo();

private:
	void generate_numbers();
};

#endif // PRNG_MT19937_HPP