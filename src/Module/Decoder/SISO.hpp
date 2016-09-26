#ifndef SISO_HPP_
#define SISO_HPP_

#include <string>
#include <vector>
#include "Tools/MIPP/mipp.h"

template <typename R>
class SISO_interface // please use SISO<R> for inheritance (instead of SISO_interface<R>)
{
protected:
	const int K_siso; // frame size
	const int N_siso; // number of information bits in the frame size
	const int n_frames_siso;

	const std::string name_siso; // module name

public:
	SISO_interface(const int K, const int N, const int n_frames = 1, std::string name = "SISO_interface") 
	: K_siso(K), N_siso(N), n_frames_siso(n_frames), name_siso(name) {}
	virtual ~SISO_interface() {};

public:
	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;
	virtual void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2) = 0;
	
	virtual int tail_length() const { return 0; }

	int get_n_frames_siso() const { return n_frames_siso; }
};

#include "SC_SISO.hpp"

#endif /* SISO_HPP_ */
