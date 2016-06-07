#ifndef CRC_HPP_
#define CRC_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class CRC
{
public:
	CRC(){};
	virtual ~CRC(){};

	virtual int  size (                          ) const = 0;
	virtual void build(      mipp::vector<B>& U_K)       = 0;
	virtual bool check(const mipp::vector<B>& V_K)       = 0;
};

#endif
