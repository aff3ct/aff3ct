/*!
 * \file
 * \brief Class module::Coset_bit.
 */
#ifndef COSET_BIT_HPP_
#define COSET_BIT_HPP_

#include "Module/Coset/Coset.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename D = int>
class Coset_bit : public Coset<B,D>
{
public:
	Coset_bit(const int size);

	virtual ~Coset_bit() = default;

	virtual Coset_bit<B,D>* clone() const;

protected:
	void _apply(const B *ref, const D *in, D *out, const size_t frame_id);
};
}
}

#endif /* COSET_BIT_HPP_ */
