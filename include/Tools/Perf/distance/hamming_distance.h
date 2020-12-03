#ifndef HAMMING_DISTANCE_H_
#define HAMMING_DISTANCE_H_

#include <cstdint>
#include <cstddef>

namespace aff3ct
{
namespace tools
{
/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 * when B is a floating point type then compute the hamming distance from their sign
 */
template <typename B = int32_t>
inline size_t hamming_distance_seq(const B *in1, const B *in2, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 * Operations are optimized with MIPP except for AVX architecture that call hamming_distance_seq.
 * when B is a floating point type then compute the hamming distance from their sign
 */
template <typename B = int32_t>
inline size_t hamming_distance(const B *in1, const B *in2, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in' and a all zero word of length 'size'
 * when B is a floating point type then compute the hamming distance from the sign
 */
template <typename B = int32_t>
inline size_t hamming_distance_seq(const B *in, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in' and a all zero word of length 'size'
 * Operations are optimized with MIPP except for AVX architecture that call hamming_distance_seq.
 * when B is a floating point type then compute the hamming distance from the sign
 */
template <typename B = int32_t>
inline size_t hamming_distance(const B *in, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 * when B is a floating point type then compute the hamming distance from their sign
 * Check if 'in' values are unknown symbol (tools::is_unknown_symbol<B>(in[i])) and then count as different
 */
template <typename B = int32_t>
inline size_t hamming_distance_unk_seq(const B *in1, const B *in2, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 * Operations are optimized with MIPP except for AVX architecture that call hamming_distance_seq.
 * when B is a floating point type then compute the hamming distance from their sign
 * Check if 'in' values are unknown symbol (tools::is_unknown_symbol<B>(in[i])) and then count as different
 */
template <typename B = int32_t>
inline size_t hamming_distance_unk(const B *in1, const B *in2, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in' and a all zero word of length 'size'
 * when B is a floating point type then compute the hamming distance from the sign
 * Check if 'in' values are unknown symbol (tools::is_unknown_symbol<B>(in[i])) and then count as different
 */
template <typename B = int32_t>
inline size_t hamming_distance_unk_seq(const B *in, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in' and a all zero word of length 'size'
 * Operations are optimized with MIPP except for AVX architecture that call hamming_distance_seq.
 * when B is a floating point type then compute the hamming distance from the sign
 * Check if 'in' values are unknown symbol (tools::is_unknown_symbol<B>(in[i])) and then count as different
 */
template <typename B = int32_t>
inline size_t hamming_distance_unk(const B *in, const unsigned size);
}
}

#include "Tools/Perf/distance/hamming_distance.hxx"

#endif /* HAMMING_DISTANCE_H_ */