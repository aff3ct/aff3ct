#ifndef ENCODER_RSC3_CPE_SYS_HPP_
#define ENCODER_RSC3_CPE_SYS_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Encoder_RSC_sys.hpp"

template <typename B>
class Encoder_RSC_G_sys : public Encoder_RSC_sys<B>
{
public:
	Encoder_RSC_G_sys(const int& K, const int& N, const int& n_frames = 1, const bool buffered_encoding = true, std::vector<int> poly = {5,7});
	virtual ~Encoder_RSC_G_sys() {}

protected:
	int inner_encode(const int bit_sys, int &state);
	int tail_bit_sys(const int &state             );
private:
	std::vector<int> out_parity;
	std::vector<int> next_state;
	std::vector<int> sys_tail;
};

#endif // ENCODER_RSC3_CPE_SYS_HPP_
