#ifndef HAMMING_DISTANCE_H_
#define HAMMING_DISTANCE_H_

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 */
template <typename B = int32_t>
size_t hamming_distance_seq(const B *in1, const B *in2, const unsigned size);

/*
 * compute the Hamming distance between the arrays 'in1' and 'in2' of length 'size'
 * Operations are optimized with MIPP except for AVX architecture that call hamming_distance_seq.
 */
template <typename B = int32_t>
size_t hamming_distance(const B *in1, const B *in2, const unsigned size);
}
}

#endif /* HAMMING_DISTANCE_H_ */