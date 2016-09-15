#ifndef SOURCE_AZCW_HPP_
#define SOURCE_AZCW_HPP_

#include <random>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Source.hpp"

template <typename B>
class Source_AZCW : public Source<B>
{
public:
	Source_AZCW(const int K, const int n_frames = 1, const std::string name = "Source_AZCW");

	virtual ~Source_AZCW();

	void generate(mipp::vector<B>& U_K);
};

#endif /* SOURCE_AZCW_HPP_ */
