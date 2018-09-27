#ifndef COSET_BIT_HPP_
#define COSET_BIT_HPP_

#include <vector>
#include <string>
#include <mipp.h>

#include "../Coset.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename D = int>
class Coset_bit : public Coset<B,D>
{
public:
	Coset_bit(const int size, const int n_frames = 1);

	virtual ~Coset_bit() = default;

protected:
	void _apply(const B *ref, const D *in, D *out, const int frame_id);
};
}
}

#endif /* COSET_BIT_HPP_ */
