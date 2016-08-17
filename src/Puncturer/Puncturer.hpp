#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename Q>
class Puncturer
{
protected:
	int n_frames;

public:
	Puncturer(const int n_frames = 1, const std::string name = "Puncturer") : n_frames(n_frames) {};
	virtual ~Puncturer() {};

	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const = 0;
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const = 0;

	virtual void set_n_frames(const int n_frames) 
	{ 
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#endif /* PUNCTURER_HPP_ */
