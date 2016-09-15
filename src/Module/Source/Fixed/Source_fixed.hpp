#ifndef SOURCE_FIXED_HPP_
#define SOURCE_FIXED_HPP_

#include <random>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Source.hpp"

template <typename B>
class Source_fixed : public Source<B>
{
public:
	Source_fixed(const int K, const int n_frames = 1, const std::string name = "Source_fixed");
	virtual ~Source_fixed();

	void generate(mipp::vector<B>& U_K);
};

#endif /* SOURCE_FIXED_HPP_ */
