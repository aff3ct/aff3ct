#ifndef PUNCTURER_NO_HPP_
#define PUNCTURER_NO_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Puncturer.hpp"

template <typename B, typename Q>
class Puncturer_NO : public Puncturer<B,Q>
{
public:
	Puncturer_NO(const int K, const int N, const int n_frames = 1, const std::string name = "Puncturer_NO");
	virtual ~Puncturer_NO();

	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const;
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const;
};

#endif /* PUNCTURER_NO_HPP_ */
