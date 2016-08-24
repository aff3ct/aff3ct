#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R, typename Q>
class Modulator_interface // please use Modulator<B,R,Q> for inheritance (instead of Modulator_interface<B,R,Q>)
{
protected:
	const int N;     // frame size
	const int N_mod; // number of elements after the modulation (could be smaller, bigger or equal to N)
	const int N_fil; // number of elements after the filtering
	      int n_frames;

	const std::string name; // module name

public:
	Modulator_interface(const int N, const int N_mod, const int N_fil, const int n_frames = 1, 
	                    const std::string name = "Modulator_interface")
	: N(N), N_mod(N_mod), N_fil(N_fil), n_frames(n_frames), name(name) {}

	Modulator_interface(const int N, const int N_mod, const int n_frames = 1, 
	                    const std::string name = "Modulator_interface")
	: N(N), N_mod(N_mod), N_fil(get_buffer_size_after_filtering(N_mod)), n_frames(n_frames), name(name) {}

	Modulator_interface(const int N, const int n_frames = 1, const std::string name = "Modulator_interface")
	: N(N), N_mod(get_buffer_size_after_modulation(N)), N_fil(get_buffer_size_after_filtering(N)), n_frames(n_frames), 
	  name(name) {}

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

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
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