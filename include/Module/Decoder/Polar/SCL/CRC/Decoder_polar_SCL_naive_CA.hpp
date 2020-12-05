/*!
 * \file
 * \brief Class module::Decoder_polar_SCL_naive_CA.
 */
#ifndef DECODER_POLAR_SCL_NAIVE_CA_
#define DECODER_POLAR_SCL_NAIVE_CA_

#include <memory>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B=int, typename R=float, tools::proto_f<R> F = tools::f_LLR, tools::proto_g<B,R> G = tools::g_LLR>
class Decoder_polar_SCL_naive_CA : public Decoder_polar_SCL_naive<B,R,F,G>
{
protected:
	std::shared_ptr<CRC<B>> crc;

public:
	Decoder_polar_SCL_naive_CA(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                           const CRC<B>& crc);

	virtual ~Decoder_polar_SCL_naive_CA() = default;

	virtual Decoder_polar_SCL_naive_CA<B,R,F,G>* clone() const;

protected:
	virtual void deep_copy(const Decoder_polar_SCL_naive_CA<B,R,F,G>& m);
	virtual void select_best_path(const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hxx"
#endif

#endif /* DECODER_POLAR_SCL_NAIVE_CA_ */
