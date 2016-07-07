#ifndef SOURCE_RANDOM_FAST_HPP_
#define SOURCE_RANDOM_FAST_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

#include "Source.hpp"

template <typename B>
class Source_random_fast : public Source<B>
{
private:
	unsigned int g_seed;

public:
	Source_random_fast();
	virtual ~Source_random_fast();

	void generate(mipp::vector<B>& U_K);

private:
	inline int random_number();
	inline int rand_max();
};

#endif /* SOURCE_RANDOM_FAST_HPP_ */
