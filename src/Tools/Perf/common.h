#ifndef COMMON_H_
#define COMMON_H_

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
void hard_decide(const Q *in, B *out, const int size);

template <typename B = int32_t>
size_t hamming_distance_seq(const B *in1, const B *in2, const int size);

template <typename B = int32_t>
size_t hamming_distance(const B *in1, const B *in2, const int size);
}
}

#endif /* COMMON_H_ */