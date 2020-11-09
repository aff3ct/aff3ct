/*!
 * \file
 * \brief Class module::Decoder_polar_SCF_naive_sys.
 */
#ifndef DECODER_POLAR_SCF_NAIVE_SYS_
#define DECODER_POLAR_SCF_NAIVE_SYS_

#include <vector>

#include "Tools/Algo/Tree/Binary/Binary_tree.hpp"
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
	                            const CRC<B>& crc, const int n_flips);
	virtual ~Decoder_polar_SCF_naive_sys() = default;

	virtual Decoder_polar_SCF_naive_sys<B,R,F,G,H>* clone() const;

protected:
	void _store   (B *V, bool coded = false) const;
	bool check_crc(const size_t frame_id   );
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCF/Decoder_polar_SCF_naive_sys.hxx"
#endif

#endif /* DECODER_POLAR_SCF_NAIVE_SYS_ */
