/*!
 * \file
 * \brief Class module::Decoder_repetition_std.
 */
#ifndef DECODER_REPETITION_STD
#define DECODER_REPETITION_STD

#include "Module/Decoder/Repetition/Decoder_repetition.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_repetition_std : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding = true);
	virtual ~Decoder_repetition_std() = default;
	virtual Decoder_repetition_std<B,R>* clone() const;

protected:
	int _decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id);
};
}
}

#endif /* DECODER_REPETITION_STD */
