#ifndef COMPUTE_PARITY_H_
#define COMPUTE_PARITY_H_

#include <mipp.h>

namespace aff3ct
{
namespace tools
{

template <typename B = int>
inline B compute_parity(const B *in, const int size)
{
	B parity = 0;
	for (int i = 0; i < size; i++)
		parity ^= in[i];

	return parity;
}

}
}

#endif /* COMPUTE_PARITY_H_ */
