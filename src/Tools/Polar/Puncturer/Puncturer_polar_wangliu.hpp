#ifndef PUNCTURER_POLAR_WANGLIU_HPP_
#define PUNCTURER_POLAR_WANGLIU_HPP_

#include "../../MIPP/mipp.h"

#include "../Frozenbits_generator/Frozenbits_generator.hpp"

#include "Puncturer_polar.hpp"

template <typename B, typename R>
class Puncturer_polar_wangliu : public Puncturer_polar<B,R>
{
protected:
	const int&      N;
	const int       N_2; // next power of two of N
	const int       Np;  // punctured bits number
	const int&      K;
	mipp::vector<B> puncturing_pattern;

	const Frozenbits_generator<B> &fb_generator;

public:
	Puncturer_polar_wangliu(const int& N, const int& K, const Frozenbits_generator<B> &fb_generator);
	virtual ~Puncturer_polar_wangliu();

	virtual void gen_frozen_bits(mipp::vector<B>& frozen_bits);
	virtual void puncture       (mipp::vector<R>& Y_N) const;

};

#endif /* PUNCTURER_POLAR_WANGLIU_HPP_ */