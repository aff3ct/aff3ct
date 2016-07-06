/* 
 * The Mersenne Twister pseudo-random number generator (PRNG)
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
 * srand().  If you need to mix them, you should encapsulate this code in a
 * namespace.
 *
 * Written by Christian Stigen Larsen
 * http://csl.name
 *
 * Distributed under the modified BSD license.
 *
 * 2015-02-17
 */

#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Maximum number you can get from rand().
 */
#define RAND_MAX INT32_MAX

/*
 * Initialize the number generator with given seed.
 * (LIBC REPLACEMENT FUNCTION)
 */
void srand(unsigned seed_value);

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
 * Initialize Mersenne Twister with given seed value.
 */
void seed(uint32_t seed_value);

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

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MERSENNE_TWISTER_H

