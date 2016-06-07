#ifndef SOURCE_RANDOM_HPP_
#define SOURCE_RANDOM_HPP_

#include <random>
#include <vector>
#include "../Tools/MIPP/mipp.h"

#include "Source.hpp"

template <typename B>
class Source_random : public Source<B>
{
private:
	std::random_device rd;
	std::linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647> rd_engine;
	//std::subtract_with_carry_engine<std::uint_fast32_t, 24, 10, 24> rd_engine;
	std::uniform_int_distribution<B> uniform_dist;

public:
	Source_random(const int seed = 0);

	virtual ~Source_random();

	void generate(mipp::vector<B>& U_K);
};

#endif /* SOURCE_RANDOM_HPP_ */
