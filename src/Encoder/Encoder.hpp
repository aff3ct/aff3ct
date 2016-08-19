#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class Encoder_interface
{
protected:
	const int K; // frame size
	const int N; // number of information bits in the frame size
	      int n_frames;

public:
	Encoder_interface(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_interface") 
	: K(K), N(N), n_frames(n_frames) {};
	virtual ~Encoder_interface() {};

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

	virtual int tail_length() const { return 0; }

	virtual void set_n_frames(const int n_frames) 
	{ 
		assert(n_frames > 0);
		this->n_frames = n_frames; 
	}
};

#include "SC_Encoder.hpp"

#endif /* ENCODER_HPP_ */
