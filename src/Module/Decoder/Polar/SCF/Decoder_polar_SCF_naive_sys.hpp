#ifndef DECODER_POLAR_SCF_NAIVE_SYS_
#define DECODER_POLAR_SCF_NAIVE_SYS_

#include <vector>

#include "Tools/Algo/Tree/Binary_tree.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar/SCF/Decoder_polar_SCF_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_f<  R> F = tools::f_LLR,
                                                tools::proto_g<B,R> G = tools::g_LLR,
                                                tools::proto_h<B,R> H = tools::h_LLR>
class Decoder_polar_SCF_naive_sys : public Decoder_polar_SCF_naive<B,R,F,G,H>
{
public:
	Decoder_polar_SCF_naive_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                            CRC<B>& crc, const int n_flips, const int n_frames = 1);
	virtual ~Decoder_polar_SCF_naive_sys() = default;

protected:
	void _store   (B *V, bool coded = false) const;
	bool check_crc(                        );
};
}
}

#include "Module/Decoder/Polar/SCF/Decoder_polar_SCF_naive_sys.hxx"

#endif /* DECODER_POLAR_SCF_NAIVE_SYS_ */
