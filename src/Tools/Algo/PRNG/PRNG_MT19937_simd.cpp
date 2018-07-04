#include <limits>

#include "PRNG_MT19937_simd.hpp"

using namespace aff3ct::tools;

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
	m = mipp::blend(MATRIX[1], MATRIX[0], (y & 1) == 1); \
	MT[i] = MT[expr] ^ (y >> 1) ^ m; \
	++i;

PRNG_MT19937_simd::PRNG_MT19937_simd(const mipp::Reg<int32_t> seed)
: MT(SIZE), index(0)
{
	this->seed(seed);
}

PRNG_MT19937_simd::PRNG_MT19937_simd()
: MT(SIZE), index(0)
{
	mipp::vector<int32_t> seed(mipp::nElReg<int32_t>());
	for (auto i = 0; i < mipp::N<int32_t>(); i++)
		seed[i] = i;
	this->seed(seed.data());
}

PRNG_MT19937_simd::~PRNG_MT19937_simd()
{
}

void PRNG_MT19937_simd::seed(const mipp::Reg<int32_t> seed)
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

void PRNG_MT19937_simd::generate_numbers()
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
	mipp::Reg<int32_t> MATRIX[2] = {mipp::Reg<int32_t>(0), mipp::Reg<int32_t>(0x9908b0df)};
	mipp::Reg<int32_t> y, m;
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
	m = mipp::blend(MATRIX[1], MATRIX[0], (y & 1) == 1);
	MT[SIZE-1] = MT[PERIOD-1] ^ (y >> 1) ^ m;
}

mipp::Reg<int32_t> PRNG_MT19937_simd::rand_s32()
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

mipp::Reg<float> PRNG_MT19937_simd::randf_cc()
{
	mipp::Reg<int32_t> rand_s32 = this->rand_s32();
	mipp::Reg<float>   max      = (float)std::numeric_limits<int32_t>::max();

	return mipp::abs(rand_s32.cvt<float>() / max);
}

mipp::Reg<float> PRNG_MT19937_simd::randf_co()
{
	mipp::Reg<int32_t> rand_s32 = this->rand_s32();
	mipp::Reg<float>   max      = (float)std::numeric_limits<int32_t>::max();

	return mipp::abs(rand_s32.cvt<float>() / (max + 1.0f));
}

mipp::Reg<float> PRNG_MT19937_simd::randf_oo()
{
	mipp::Reg<int32_t> rand_s32 = this->rand_s32();
	mipp::Reg<float>   max      = (float)std::numeric_limits<int32_t>::max();

	return mipp::abs((rand_s32.cvt<float>() + 0.5f) / (max + 1.0f));
}
