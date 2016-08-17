#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class Encoder
{
protected:
	int n_frames;

public:
	Encoder(const int n_frames = 1, const std::string name = "Encoder") : n_frames(n_frames) {};
	virtual ~Encoder() {};

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

	virtual int tail_length() const { return 0; }

	virtual void set_n_frames(const int n_frames) 
	{ 
		assert(n_frames > 0);
		this->n_frames = n_frames; 
	}
};

#endif /* ENCODER_HPP_ */
