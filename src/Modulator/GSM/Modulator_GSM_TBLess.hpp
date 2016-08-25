#ifndef MODULATOR_GSM_TBLESS_HPP_
#define MODULATOR_GSM_TBLESS_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"
#include "../../Tools/params.h"
#include "../../Tools/Math/max.h"

#include "Modulator_GSM.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
class Modulator_GSM_TBLess : public Modulator_GSM<B,R,Q,MAX>
{
public:
	Modulator_GSM_TBLess(int N, const R sigma, const bool disable_sig2 = false, 
	                     const std::string name = "Modulator_GSM_TBLess");
	virtual ~Modulator_GSM_TBLess();

	void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2);
	void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2);
	void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);

	int get_buffer_size_after_modulation(const int N);
	int get_buffer_size_after_filtering (const int N);
};

#include "Modulator_GSM_TBLess.hxx"

#endif /* MODULATOR_GSM_TBLESS_HPP_ */
