/*!
 * \file
 * \brief Class module::Coset_real.
 */
#ifndef COSET_REAL_HPP_
#define COSET_REAL_HPP_

#include "Module/Coset/Coset.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename D = float>
class Coset_real : public Coset<B,D>
{
public:
	Coset_real(const int size);

	virtual ~Coset_real() = default;

	virtual Coset_real<B,D>* clone() const;

protected:
	void _apply(const B *ref, const D *in, D *out, const size_t frame_id);
};
}
}

#endif /* COSET_BIT_HPP_ */
