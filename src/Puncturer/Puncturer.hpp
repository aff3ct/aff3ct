#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename Q>
class Puncturer_interface // please use Puncturer<B,Q> for inheritance (instead of Puncturer_interface<B,Q>)
{
protected:
	const int K;      // number of information bits
	const int N;      // frame size
	const int N_code; // real size of the code (N_code >= N)

	int n_frames;

public:
	Puncturer_interface(const int K, const int N, const int N_code, const int n_frames = 1, 
	                    const std::string name = "Puncturer_interface") 
	: K(K), N(N), N_code(N_code), n_frames(n_frames) {};
	virtual ~Puncturer_interface() {};

	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const = 0;
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const = 0;

	virtual void set_n_frames(const int n_frames) 
	{ 
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#include "SC_Puncturer.hpp"

#endif /* PUNCTURER_HPP_ */
