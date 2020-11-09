/*!
 * \file
 * \brief Class module::Puncturer_polar_shortlast.
 */
#ifndef PUNCTURER_POLAR_SHORTLAST_HPP_
#define PUNCTURER_POLAR_SHORTLAST_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_polar_shortlast : public Puncturer<B,Q>
{
protected:
	const tools::Frozenbits_generator &fb_generator;

public:
	Puncturer_polar_shortlast(const int &K,
	                          const int &N,
	                          const tools::Frozenbits_generator &fb_generator);
	virtual ~Puncturer_polar_shortlast() = default;

	virtual Puncturer_polar_shortlast<B,Q>* clone() const;

	void gen_frozen_bits(std::vector<bool> &frozen_bits);

protected:
	void   _puncture(const B *X_N1, B *X_N2, const size_t frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const;
};
}
}

#endif /* PUNCTURER_POLAR_SHORTLAST_HPP_ */
