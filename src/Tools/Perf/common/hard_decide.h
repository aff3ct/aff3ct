#ifndef HARD_DECIDE_H_
#define HARD_DECIDE_H_

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

}
}

#endif /* HARD_DECIDE_H_ */