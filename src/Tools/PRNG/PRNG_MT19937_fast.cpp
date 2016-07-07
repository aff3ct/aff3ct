#include <stdio.h>

#include "PRNG_MT19937_fast.hpp"

/*
 * We have an array of 624 32-bit values, and there are
 * 31 unused bits, so we have a seed value of
 * 624*32-31 = 19937 bits.
 */
constexpr unsigned SIZE   = 624;
constexpr unsigned PERIOD = 397;
constexpr unsigned DIFF   = SIZE-PERIOD;

#define M32(x) (x & 0x80000000) // 32nd Most Significant Bit
#define L31(x) (x & 0x7FFFFFFF) // 31 Least Significant Bits

#define UNROLL(expr) \
	y = M32(MT[i]) | L31(MT[i+1]); \
	m = ((y & 1) == 1) & 0x9908b0df; \
	MT[i] = MT[expr] ^ (y >> 1) ^ m; \
	++i;

PRNG_MT19937_fast::PRNG_MT19937_fast(const mipp::Reg<int> seed)
: MT(SIZE), index(0)
{
	this->seed(seed);
}

PRNG_MT19937_fast::PRNG_MT19937_fast()
: MT(SIZE), index(0)
{
	mipp::vector<int> seed(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seed[i] = i;
	this->seed(seed.data());
}

PRNG_MT19937_fast::~PRNG_MT19937_fast()
{
}

void PRNG_MT19937_fast::seed(const mipp::Reg<int> seed)
{
	/*
	 * The equation below is a linear congruential generator (LCG),
	 * one of the oldest known pseudo-random number generator
	 * algorithms, in the form X_(n+1) = = (a*X_n + c) (mod m).
	 *
	 * We've implicitly got m=32 (mask + word size of 32 bits), so
	 * there is no need to explicitly use modulus.
	 *
	 * What is interesting is the multiplier a.  The one we have
	 * below is 0x6c07865 --- 1812433253 in decimal, and is called
	 * the Borosh-Niederreiter multiplier for modulus 2^32.
	 *
	 * It is mentioned in passing in Knuth's THE ART OF COMPUTER
	 * PROGRAMMING, Volume 2, page 106, Table 1, line 13.  LCGs are
	 * treated in the same book, pp. 10-26
	 *
	 * You can read the original paper by Borosh and Niederreiter
	 * as well.  It's called OPTIMAL MULTIPLIERS FOR PSEUDO-RANDOM
	 * NUMBER GENERATION BY THE LINEAR CONGRUENTIAL METHOD (1983) at
	 * http://www.springerlink.com/content/n7765ku70w8857l7/
	 *
	 * You can read about LCGs at:
	 * http://en.wikipedia.org/wiki/Linear_congruential_generator
	 *
	 * From that page, it says:
	 * "A common Mersenne twister implementation, interestingly
	 * enough, uses an LCG to generate seed data.",
	 *
	 * Since we're using 32-bits data types for our MT array, we can skip the
	 * masking with 0xFFFFFFFF below.
	 */

	MT[0] = seed;
	index = 0;

	for (unsigned i = 1; i < SIZE; ++i)
		MT[i] = (MT[i-1] ^ MT[i-1] >> 30) * 0x6c078965 +i;
}

void PRNG_MT19937_fast::generate_numbers()
{
	/*
	 * Originally, we had one loop with i going from [0, SIZE) and
	 * two modulus operations:
	 *
	 * for ( register unsigned i=0; i<SIZE; ++i ) {
	 *   register uint32_t y = M32(MT[i]) | L31(MT[(i+1) % SIZE]);
	 *   MT[i] = MT[(i + PERIOD) % SIZE] ^ (y>>1);
	 *   if ( ODD(y) ) MT[i] ^= 0x9908b0df;
	 * }
	 *
	 * For performance reasons, we've unrolled the loop three times, thus
	 * mitigating the need for any modulus operations.
	 *
	 * Anyway, it seems this trick is old hat:
	 * http://www.quadibloc.com/crypto/co4814.htm
	 *
	 */

	mipp::Reg<int> y, m;
	uint32_t i = 0;

	// i = [0 ... 225]
	while (i < (DIFF -1)) 
	{
		/*
		 * We're doing 226 = 113*2, an even number of steps, so we can
		 * safely unroll one more step here for speed:
		 */
		UNROLL(i+PERIOD);
		UNROLL(i+PERIOD);
	}

	// i = 226
	UNROLL((i+PERIOD) % SIZE);

	// i = [227 ... 622]
	while (i < (SIZE-1)) 
	{
		/*
		 * 623-227 = 396 = 2*2*3*3*11, so we can unroll this loop in any number
		 * that evenly divides 396 (2, 4, 6, etc). Here we'll unroll 11 times.
		 */
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
		UNROLL(i-DIFF);
	}

	// i = 623
	y = M32(MT[SIZE-1]) | L31(MT[0]);
	m = ((y & 1) == 1) & 0x9908b0df;
	MT[SIZE-1] = MT[PERIOD-1] ^ (y >> 1) ^ m;
}

mipp::Reg<int> PRNG_MT19937_fast::rand_s32()
{
	if (!index)
		generate_numbers();

	auto y = MT[index];

	// Tempering
	y ^= y >> 11;
	y ^= y <<  7 & 0x9d2c5680;
	y ^= y << 15 & 0xefc60000;
	y ^= y >> 18;

	if (++index == SIZE)
		index = 0;

	return y;
}

mipp::Reg<int> PRNG_MT19937_fast::rand()
{
	/*
	 * PORTABILITY WARNING:
	 *
	 * rand_u32() uses all 32-bits for the pseudo-random number,
	 * but rand() must return a number from 0 ... RAND_MAX.
	 *
	 * We'll just assume that rand() only uses 31 bits worth of
	 * data, and that we're on a two's complement system.  
	 *
	 * So, to output an integer compatible with rand(), we have
	 * two options: Either mask off the highest (32nd) bit, or
	 * shift right by one bit.  Masking with 0x7FFFFFFF will be
	 * compatible with 64-bit MT[], so we'll just use that here.
	 *
	 */
	// return static_cast<int>(0x7FFFFFFF & rand_u32());
	return rand_s32() & 0x7FFFFFFF;
}

// mipp::Reg<float> PRNG_MT19937_fast::randf_cc()
// {
// 	return static_cast<float>(rand_u32()) / UINT32_MAX;
// }

// mipp::Reg<float> PRNG_MT19937_fast::randf_co()
// {
// 	return static_cast<float>(rand_u32()) / (UINT32_MAX +1.0f);
// }

mipp::Reg<float> PRNG_MT19937_fast::randf_oo()
{
	auto i = rand_s32();
	auto f = i.cvt<float>() + 0.5f;
	auto max = mipp::Reg<float>((float)INT32_MAX +1.0f);
	return mipp::abs(f / max);

	// return (static_cast<float>(rand_u32()) +0.5f) / (UINT32_MAX +1.0f);
}

// mipp::Reg<double> PRNG_MT19937_fast::randd_cc()
// {
// 	return static_cast<double>(rand_u32()) / UINT32_MAX;
// }

// mipp::Reg<double> PRNG_MT19937_fast::randd_co()
// {
// 	return static_cast<double>(rand_u32()) / (UINT32_MAX +1.0);
// }

// mipp::Reg<double> PRNG_MT19937_fast::randd_oo()
// {
// 	return (static_cast<double>(rand_u32()) +0.5) / (UINT32_MAX +1.0);
// }

// mipp::Reg<long long> PRNG_MT19937_fast::rand_u64()
// {
// 	return static_cast<uint64_t>(rand_u32()) << 32 | rand_u32();
// }