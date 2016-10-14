#ifndef SOURCE_USER_HPP_
#define SOURCE_USER_HPP_

#include <string>
#include <random>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Source.hpp"

template <typename B>
class Source_user : public Source<B>
{
private:
	mipp::vector<mipp::vector<B>> source;
	int src_counter;

public:
	Source_user(const int K, std::string filename, const int n_frames = 1, const std::string name = "Source_user");
	virtual ~Source_user();

	void generate(mipp::vector<B>& U_K);
};

#endif /* SOURCE_USER_HPP_ */
