#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include <string>

#include "../Tools/MIPP/mipp.h"

template <typename B>
class Source_interface // please use Source<B> for inheritance (instead of Source_interface<B>)
{
public:
	Source_interface(const std::string name = "Source_interface") {};
	virtual ~Source_interface() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;
};

#include "SC_Source.hpp"

#endif /* SOURCE_HPP_ */