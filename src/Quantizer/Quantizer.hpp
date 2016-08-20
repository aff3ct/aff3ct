#ifndef QUANTIZER_HPP_
#define QUANTIZER_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R, typename Q>
class Quantizer_interface // please use Quantizer<R,Q> for inheritance (instead of Quantizer_interface<R,Q>)
{
protected:
	const int N; // frame size
	      int n_frames;
public:
	Quantizer_interface(const int N, const int n_frames = 1, const std::string name = "Quantizer_interface")
	: N(N), n_frames(n_frames) {};
	virtual ~Quantizer_interface() {};

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#include "SC_Quantizer.hpp"

#endif /* QUANTIZER_HPP_ */
