#ifndef ENCODER_POLAR_HPP_
#define ENCODER_POLAR_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar : public Encoder<B>
{
protected:
	const int              m;           // log_2 of code length
	const mipp::vector<B>& frozen_bits; // true means frozen, false means set to 0/1
	mipp::vector<B>        U_N;

public:
	Encoder_polar(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1, 
	              const std::string name = "Encoder_polar");
	virtual ~Encoder_polar() {}

	void light_encode(B *bits);

protected:
	virtual void _encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);

	void frame_encode(const mipp::vector<B>& U_N, mipp::vector<B>& X_N, const int &i_frame = 0);
	void convert(const mipp::vector<B>& U_K, mipp::vector<B>& U_N);
};
}
}

#endif // ENCODER_POLAR_HPP_
