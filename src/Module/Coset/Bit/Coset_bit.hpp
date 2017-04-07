#ifndef COSET_BIT_HPP_
#define COSET_BIT_HPP_

#include <vector>
#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Coset.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename D = int>
class Coset_bit : public Coset<B,D>
{
public:
	Coset_bit(const int size, const int n_frames = 1, const std::string name = "Coset_bit");

	virtual ~Coset_bit();

	virtual void apply(const B *ref, const D *in_data, D *out_data); using Coset<B,D>::apply;
};
}
}

#endif /* COSET_BIT_HPP_ */
