#ifndef COSET_REAL_HPP_
#define COSET_REAL_HPP_

#include <vector>
#include <string>
#include <mipp.h>

#include "../Coset.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename D = float>
class Coset_real : public Coset<B,D>
{
public:
	Coset_real(const int size, const int n_frames = 1, const std::string name = "Coset_real");

	virtual ~Coset_real();

	virtual void apply(const B *ref, const D *in_data, D *out_data); using Coset<B,D>::apply;
};
}
}

#endif /* COSET_BIT_HPP_ */
