#ifndef COMMON_H_
#define COMMON_H_

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
/*
 * take the hard decision on the array 'in' and fill 'out', both of length 'size'
 */
template <typename B = int, typename Q = float>
void hard_decide(const Q *in, B *out, const int size);

/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 */
template <typename B = int32_t>
size_t hamming_distance_seq(const B *in1, const B *in2, const int size);

/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 * Operations are optimized with MIPP except for AVX architecture that call hamming_distance_seq.
 */
template <typename B = int32_t>
size_t hamming_distance(const B *in1, const B *in2, const int size);
}
}

#endif /* COMMON_H_ */