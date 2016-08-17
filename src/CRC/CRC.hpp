#ifndef CRC_HPP_
#define CRC_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class CRC
{
protected:
	int n_frames;

public:
	CRC(const int n_frames = 1, const std::string name = "CRC") : n_frames(n_frames) {};
	virtual ~CRC(){};

	virtual int  size (                          ) const = 0;
	virtual void build(      mipp::vector<B>& U_K)       = 0;
	virtual bool check(const mipp::vector<B>& V_K)       = 0;

	void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#endif
