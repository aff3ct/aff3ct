#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B>
class Source_interface : public Module // please use Source<B> for inheritance (instead of Source_interface<B>)
{
protected:
	const int K;

public:
	Source_interface(const int K, const int n_frames = 1, const std::string name = "Source_interface") 
	: Module(n_frames, name), K(K) {}
	virtual ~Source_interface() {}

	virtual void generate(mipp::vector<B>& U_K) = 0;
};

#include "SC_Source.hpp"

#endif /* SOURCE_HPP_ */
