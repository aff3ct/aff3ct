#ifndef SISO_HPP_
#define SISO_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R>
class SISO
{
public:
	SISO() {};
	virtual ~SISO() {};

public:
	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;
	
	virtual int get_n_frames() const { return 1; }
	virtual int tail_length () const { return 0; }
};

#endif /* SISO_HPP_ */
