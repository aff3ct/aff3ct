#ifndef CRC_HPP_
#define CRC_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class CRC_interface // please use CRC<B> for inheritance (instead of CRC_interface<B>)
{
protected:
	const int K; // number of information bit (the CRC bits are incuded in K)
	      int n_frames;

public:
	CRC_interface(const int K, const int n_frames = 1, const std::string name = "CRC_interface") 
	: K(K), n_frames(n_frames) {};
	virtual ~CRC_interface(){};

	virtual int  size (                          ) const = 0;
	virtual void build(      mipp::vector<B>& U_K)       = 0;
	virtual bool check(const mipp::vector<B>& V_K)       = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#include "SC_CRC.hpp"

#endif
