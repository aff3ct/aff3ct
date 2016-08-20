#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class Modulator_interface // please use Modulator<B,R> for inheritance (instead of Modulator_interface<B,R>)
{
protected:
	const int N; // frame size
	      int n_frames;

public:
	Modulator_interface(const int N, const int n_frames = 1, const std::string name = "Modulator_interface") 
	: N(N), n_frames(n_frames) {};
	virtual ~Modulator_interface() {};

	virtual void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2) = 0;
	virtual void     filter(const mipp::vector<R>& Y_N1,                              mipp::vector<R>& Y_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2) = 0;
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
	{
		demodulate(Y_N1, Y_N3);
	}

	virtual int get_buffer_size(const int N) { return N; }

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#include "SC_Modulator.hpp"

#endif /* MODULATOR_HPP_ */
