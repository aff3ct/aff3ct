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

PRNG_MT19937_fast::PRNG_MT19937_fast(const mipp::Reg<int> seed)
: MT(SIZE), index(0)
{
	MT[0] = seed;
	index = 0;

	const mipp::Reg<int> factor = 0x6c078965;
	for (unsigned i = 1; i < SIZE; ++i)
		MT[i] = factor * (MT[i-1] ^ MT[i-1] >> 30) +i;
}

PRNG_MT19937_fast::~PRNG_MT19937_fast()
{
}

void PRNG_MT19937_fast::generate_numbers()
{
	// constexpr uint32_t MATRIX[2] = {0, 0x9908b0df};
	mipp::Reg<int> y, m;
	uint32_t i = 0;

	mipp::Reg<int> M32  = 0x80000000; // 32nd Most Significant Bit
	mipp::Reg<int> L31  = 0x7FFFFFFF; // 31 Least Significant Bits
	mipp::Reg<int> one  = 1;
	mipp::Reg<int> zero = 0;

	auto unroll = [&](uint32_t j) 
	{
		y = (MT[i] & M32) | (MT[i+1] & L31);
		m = ((y & one) != zero) & 0x9908b0df;
		MT[i] = MT[j] ^ (y >> 1) ^ m;
		++i;
	};

	// i = [0 ... 225]
	while (i < (DIFF -1)) 
	{
	 	// We're doing 226 = 113*2, an even number of steps, so we can
	 	// safely unroll one more step here for speed:
		unroll(i+PERIOD);
		unroll(i+PERIOD);
	}

	// i = 226
	unroll((i+PERIOD) % SIZE);

	// i = [227 ... 622]
	while (i < (SIZE-1)) 
	{
		// 623-227 = 396 = 2*2*3*3*11, so we can unroll this loop in any number
		// that evenly divides 396 (2, 4, 6, etc). Here we'll unroll 11 times.
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
		unroll(i-DIFF);
	}

	// i = 623
	y = (MT[SIZE-1] & M32) | (MT[0] & L31);
	m = ((y & one) != zero) & 0x9908b0df;
	MT[SIZE-1] = MT[PERIOD-1] ^ (y >> 1) ^ m;
}

mipp::Reg<int> PRNG_MT19937_fast::rand_u32()
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
	return rand_u32();
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
	auto i = (rand_u32() >> 2);
	mipp::Reg<float> c = i.r;
	// mipp::Reg<float> max = (float)UINT32_MAX +1.0f;
	// c = (c + 0.5f) / max;

	return c;
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
