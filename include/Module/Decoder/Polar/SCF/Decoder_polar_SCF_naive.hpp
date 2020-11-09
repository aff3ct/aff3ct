/*!
 * \file
 * \brief Class module::Decoder_polar_SCF_naive.
 */
#ifndef DECODER_POLAR_SCF_NAIVE_
#define DECODER_POLAR_SCF_NAIVE_

#include <memory>
#include <vector>

#include "Tools/Algo/Tree/Binary/Binary_node.hpp"
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
	std::shared_ptr<CRC<B>> crc;

	const int n_flips;
	std::vector<int> index;
	int current_flip_index;

public:
	Decoder_polar_SCF_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                        const CRC<B>& crc, const int n_flips);
	virtual ~Decoder_polar_SCF_naive() = default;

	virtual Decoder_polar_SCF_naive<B,R,F,G,H>* clone() const;

protected:
	virtual void deep_copy(const Decoder_polar_SCF_naive<B,R,F,G,H>& m);

	virtual bool check_crc       (const size_t frame_id                                );
	        int  _decode_siho    (const R *Y_N, B *V_K, const size_t frame_id          );
	        int  _decode_siho_cw (const R *Y_N, B *V_N, const size_t frame_id          );
            void recursive_decode(const tools::Binary_node<Contents_SC<B,R>>* node_curr);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCF/Decoder_polar_SCF_naive.hxx"
#endif

#endif /* DECODER_POLAR_SCF_NAIVE_ */
