#ifndef PUNCTURER_POLAR_SHORTLAST_HPP_
#define PUNCTURER_POLAR_SHORTLAST_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Puncturer.hpp"

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
	                        const tools::Frozenbits_generator &fb_generator,
	                        const int n_frames = 1);
	virtual ~Puncturer_polar_shortlast() = default;

	void gen_frozen_bits(std::vector<bool> &frozen_bits);

protected:
	void   _puncture(const B *X_N1, B *X_N2, const int frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}

#endif /* PUNCTURER_POLAR_SHORTLAST_HPP_ */
