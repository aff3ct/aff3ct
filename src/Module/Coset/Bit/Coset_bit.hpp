#ifndef COSET_BIT_HPP_
#define COSET_BIT_HPP_

#include <vector>
#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Coset.hpp"

namespace aff3ct
{
template <typename B, typename D>
class Coset_bit : public Coset<B,D>
{
public:
	Coset_bit(const int size, const int n_frames = 1, const std::string name = "Coset_bit");

	virtual ~Coset_bit();

	virtual void apply(const mipp::vector<B>& ref, const mipp::vector<D> &in_data, mipp::vector<D> &out_data);
};
}

#endif /* COSET_BIT_HPP_ */
