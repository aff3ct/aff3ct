#ifndef SISO_HPP_
#define SISO_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename R>
class SISO_interface : public Module // please use SISO<R> for inheritance (instead of SISO_interface<R>)
{
protected:
	const int K_siso; // frame size
	const int N_siso; // number of information bits in the frame size

public:
	SISO_interface(const int K, const int N, const int n_frames = 1, std::string name = "SISO_interface") 
	: Module(n_frames, name), K_siso(K), N_siso(N) {}
	virtual ~SISO_interface() {};

public:
	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;
	virtual void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2) = 0;
	
	virtual int tail_length() const { return 0; }
};

#include "SC_SISO.hpp"

#endif /* SISO_HPP_ */
