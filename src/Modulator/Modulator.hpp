#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R, typename Q>
class Modulator
{
public:
	Modulator(const std::string name = "Modulator") {};
	virtual ~Modulator() {};

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
void Modulator<B,R,Q>
::filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	Y_N2 = Y_N1;
}

#endif /* MODULATOR_HPP_ */
