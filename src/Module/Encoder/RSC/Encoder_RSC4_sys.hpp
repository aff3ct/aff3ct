#ifndef ENCODER_RSC4_SYS_HPP_
#define ENCODER_RSC4_SYS_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Encoder_RSC_sys.hpp"

template <typename B>
class Encoder_RSC4_sys : public Encoder_RSC_sys<B>
{
public:
	Encoder_RSC4_sys(const int& K, const int& N, const int& n_frames = 1, const bool buffered_encoding = true,
	                 const std::string name = "Encoder_RSC4_sys");
	virtual ~Encoder_RSC4_sys() {}

protected:
	int inner_encode(const int bit_sys, int &state);
	int tail_bit_sys(const int &state             );
};

#endif // ENCODER_RSC4_SYS_HPP_
