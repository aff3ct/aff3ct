#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include <string>

#include "../Tools/MIPP/mipp.h"

#include "Source_SC.hpp"

template <typename B>
class Source : public Source_SC<B>
{
public:
	Source(const std::string name = "Source") : Source_SC<B>(name) {};
	virtual ~Source() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;
};

#endif /* SOURCE_HPP_ */
