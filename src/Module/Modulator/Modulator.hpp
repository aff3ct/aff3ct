#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B, typename R, typename Q>
class Modulator_interface : public Module// please use Modulator<B,R,Q> for inheritance (instead of Modulator_interface<B,R,Q>)
{
protected:
	const int N;     // frame size
	const int N_mod; // number of elements after the modulation (could be smaller, bigger or equal to N)
	const int N_fil; // number of elements after the filtering

public:
	Modulator_interface(const int N, const int N_mod, const int N_fil, const int n_frames = 1, 
	                    const std::string name = "Modulator_interface")
	: Module(n_frames, name), N(N), N_mod(N_mod), N_fil(N_fil) {}

	Modulator_interface(const int N, const int N_mod, const int n_frames = 1, 
	                    const std::string name = "Modulator_interface")
	: Module(n_frames, name), N(N), N_mod(N_mod), N_fil(get_buffer_size_after_filtering(N_mod)) {}

	Modulator_interface(const int N, const int n_frames = 1, const std::string name = "Modulator_interface")
	: Module(n_frames, name), N(N), N_mod(get_buffer_size_after_modulation(N)),
	  N_fil(get_buffer_size_after_filtering(N)) {}

	virtual ~Modulator_interface() {};

	virtual void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2) = 0;
	virtual void     filter(const mipp::vector<R>& Y_N1,                              mipp::vector<R>& Y_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2) = 0;
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
	{
		demodulate(Y_N1, Y_N3);
	}

	virtual int get_buffer_size_after_modulation(const int N) { return N;                                   }
	virtual int get_buffer_size_after_filtering (const int N) { return get_buffer_size_after_modulation(N); }
};

template <typename B, typename R, typename Q>
void Modulator_interface<B,R,Q>
::filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	Y_N2 = Y_N1;
}

#include "SC_Modulator.hpp"

#endif /* MODULATOR_HPP_ */
