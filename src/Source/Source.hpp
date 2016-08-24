#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include <string>

#include "../Tools/MIPP/mipp.h"

template <typename B>
class Source_interface // please use Source<B> for inheritance (instead of Source_interface<B>)
{
protected:
	const int K;
	      int n_frames;

	const std::string name; // module name

public:
	Source_interface(const int K, const int n_frames = 1, const std::string name = "Source_interface") 
	: K(K), n_frames(n_frames), name(name) {};
	virtual ~Source_interface() {};

	virtual void generate(mipp::vector<B>& U_K) = 0;

	virtual void set_n_frames(const int n_frames) 
	{ 
		assert(n_frames > 0);
		this->n_frames = n_frames; 
	}
};

#include "SC_Source.hpp"

#endif /* SOURCE_HPP_ */