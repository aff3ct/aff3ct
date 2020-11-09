/*!
 * \file
 * \brief Class module::Decoder_NO.
 */
#ifndef DECODER_NO_HPP_
#define DECODER_NO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_NO : public Decoder_SISO<B,R>
{
public:
	Decoder_NO(const int K);
	virtual ~Decoder_NO() = default;
	virtual Decoder_NO<B,R>* clone() const;

protected:
	int _decode_siso   (const R *sys, const R *par, R *ext, const size_t frame_id);
	int _decode_siso   (const R *Y_K1, R *Y_K2, const size_t frame_id);
	int _decode_siho   (const R *Y_K,  B *V_K,  const size_t frame_id);
	int _decode_siho_cw(const R *Y_K,  B *V_K,  const size_t frame_id);
};
}
}

#endif /* DECODER_NO_HPP_ */
