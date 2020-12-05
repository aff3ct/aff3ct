/*!
 * \file
 * \brief Class module::Decoder_polar_SCL_naive_CA_sys.
 */
#ifndef DECODER_POLAR_SCL_NAIVE_CA_SYS_
#define DECODER_POLAR_SCL_NAIVE_CA_SYS_

#include <vector>

#include "Module/CRC/CRC.hpp"
#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B=int, typename R=float, tools::proto_f<R> F = tools::f_LLR, tools::proto_g<B,R> G = tools::g_LLR>
class Decoder_polar_SCL_naive_CA_sys : public Decoder_polar_SCL_naive_CA<B,R,F,G>
{

public:
	Decoder_polar_SCL_naive_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                               const CRC<B>& crc);

	virtual ~Decoder_polar_SCL_naive_CA_sys() = default;

	virtual Decoder_polar_SCL_naive_CA_sys<B,R,F,G>* clone() const;

protected:
	virtual void select_best_path(const size_t frame_id);
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA_sys.hxx"
#endif

#endif /* DECODER_POLAR_SCL_NAIVE_CA_SYS_ */
