#ifndef DECODER_POLAR_SCF_NAIVE_
#define DECODER_POLAR_SCF_NAIVE_

#include <vector>

#include "Tools/Algo/Tree/Binary_node.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_f<  R> F = tools::f_LLR,
                                                tools::proto_g<B,R> G = tools::g_LLR,
                                                tools::proto_h<B,R> H = tools::h_LLR>
class Decoder_polar_SCF_naive : public Decoder_polar_SC_naive<B,R,F,G,H>
{
protected:
	CRC<B>& crc;

	const int n_flips;
	std::vector<int> index;
	int current_flip_index;
	std::vector<tools::Binary_node<Contents_SC<B,R>>*> leaves;
public:
	Decoder_polar_SCF_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                        CRC<B>& crc, const int n_flips, const int n_frames = 1);
	virtual ~Decoder_polar_SCF_naive() = default;

protected:
	virtual bool check_crc       (                                                     );
	        void _decode_siho    (const R *Y_N, B *V_K, const int frame_id             );
	        void _decode_siho_cw (const R *Y_N, B *V_N, const int frame_id             );
            void recursive_decode(const tools::Binary_node<Contents_SC<B,R>>* node_curr);
};
}
}

#include "Module/Decoder/Polar/SCF/Decoder_polar_SCF_naive.hxx"

#endif /* DECODER_POLAR_SCF_NAIVE_ */
