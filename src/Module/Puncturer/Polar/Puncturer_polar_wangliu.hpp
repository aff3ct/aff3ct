#ifndef PUNCTURER_POLAR_WANGLIU_HPP_
#define PUNCTURER_POLAR_WANGLIU_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Puncturer.hpp"

namespace aff3ct
{
template <typename B, typename Q>
class Puncturer_polar_wangliu : public Puncturer<B,Q>
{
protected:
	const Frozenbits_generator<B> &fb_generator;

public:
	Puncturer_polar_wangliu(const int &N, 
	                        const int &K, 
	                        const Frozenbits_generator<B> &fb_generator, 
	                        const int n_frames = 1,
	                        const std::string name = "Puncturer_polar_wangliu");
	virtual ~Puncturer_polar_wangliu();

	void gen_frozen_bits(mipp::vector<B> &frozen_bits);

	void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const;
	void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const;
};
}

#endif /* PUNCTURER_POLAR_WANGLIU_HPP_ */
