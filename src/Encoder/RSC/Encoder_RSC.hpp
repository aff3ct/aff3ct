#ifndef ENCODER_RSC_HPP_
#define ENCODER_RSC_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_RSC : public Encoder_sys<B>
{
protected:
	const int n_states;

public:
	Encoder_RSC(const int n_states) : Encoder_sys<B>(), n_states(n_states) {}
	virtual ~Encoder_RSC() {}

	virtual void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;
	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par) = 0;

	int get_n_states()       { return 1 * n_states; }
	int tail_length () const { return 2 * n_states; }
};

#endif /* ENCODER_RSC_HPP_ */
