#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class Source
{
public:
	Source() {};
	virtual ~Source() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;
};

#endif /* SOURCE_HPP_ */
