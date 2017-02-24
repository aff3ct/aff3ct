#ifndef COSET_REAL_HPP_
#define COSET_REAL_HPP_

#include <vector>
#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Coset.hpp"

namespace aff3ct
{
template <typename B, typename D>
class Coset_real : public Coset<B,D>
{
public:
	Coset_real(const int size, const int n_frames = 1, const std::string name = "Coset_real");

	virtual ~Coset_real();

	virtual void apply(const mipp::vector<B>& ref, const mipp::vector<D> &in_data, mipp::vector<D> &out_data);
};
}

#endif /* COSET_BIT_HPP_ */
