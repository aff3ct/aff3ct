#ifndef ENCODER_RSC_HPP_
#define ENCODER_RSC_HPP_

#include <cassert>
#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_RSC : public Encoder_sys<B>
{
protected:
	const int n_ff;     // number of D flip-flop
	const int n_states;
	      int n_frames;

public:
	Encoder_RSC(const int n_ff, const int n_frames = 1) 
	: Encoder_sys<B>(), n_ff(n_ff), n_states(1 << n_ff), n_frames(n_frames) {}
	virtual ~Encoder_RSC() {}

	virtual void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;
	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par) = 0;

	virtual mipp::vector<mipp::vector<int>> get_trellis() = 0;

	int get_n_ff   ()       { return n_ff;     }
	int tail_length() const { return 2 * n_ff; }

	void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#endif /* ENCODER_RSC_HPP_ */
