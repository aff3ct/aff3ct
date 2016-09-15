#ifndef QUANTIZER_HPP_
#define QUANTIZER_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename R, typename Q>
class Quantizer_interface : public Module // please use Quantizer<R,Q> for inheritance (instead of Quantizer_interface<R,Q>)
{
protected:
	const int N; // frame size

public:
	Quantizer_interface(const int N, const int n_frames = 1, const std::string name = "Quantizer_interface")
	: Module(n_frames, name), N(N) {};
	virtual ~Quantizer_interface() {};

	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;
};

#include "SC_Quantizer.hpp"

#endif /* QUANTIZER_HPP_ */
