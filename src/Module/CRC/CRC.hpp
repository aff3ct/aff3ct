#ifndef CRC_HPP_
#define CRC_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B>
class CRC_interface : public Module // please use CRC<B> for inheritance (instead of CRC_interface<B>)
{
protected:
	const int K; // number of information bit (the CRC bits are incuded in K)

public:
	CRC_interface(const int K, const int n_frames = 1, const std::string name = "CRC_interface") 
	: Module(n_frames, name), K(K) {}
	virtual ~CRC_interface() {}

	virtual int  size (                          ) const = 0;
	virtual void build(      mipp::vector<B>& U_K)       = 0;
	virtual bool check(const mipp::vector<B>& V_K)       = 0;
};

#include "SC_CRC.hpp"

#endif
