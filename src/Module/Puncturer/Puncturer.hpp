#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B, typename Q>
class Puncturer_interface : public Module // please use Puncturer<B,Q> for inheritance (instead of Puncturer_interface<B,Q>)
{
protected:
	const int K;      // number of information bits
	const int N;      // frame size
	const int N_code; // real size of the code (N_code >= N)

public:
	Puncturer_interface(const int K, const int N, const int N_code, const int n_frames = 1, 
	                    const std::string name = "Puncturer_interface") 
	: Module(n_frames, name), K(K), N(N), N_code(N_code) {}
	virtual ~Puncturer_interface() {}

	virtual void   puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const = 0;
	virtual void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const = 0;
};

#include "SC_Puncturer.hpp"

#endif /* PUNCTURER_HPP_ */
