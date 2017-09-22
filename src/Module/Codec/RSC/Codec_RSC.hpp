#ifndef CODEC_RSC_HPP_
#define CODEC_RSC_HPP_

#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Decoder/RSC/Decoder_RSC.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_RSC : public Codec_SISO_SIHO<B,Q>
{
protected:
	const bool buffered_encoding;
	std::vector<std::vector<int>> trellis;

public:
	Codec_RSC(const factory::Encoder_RSC::parameters &enc_params,
	          const factory::Decoder_RSC::parameters &dec_params,
	          const std::string name = "Codec_RSC");
	virtual ~Codec_RSC();

protected:
	void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id);
	void _extract_sys_llr(const Q *Y_N, Q *sys,         const int frame_id);
	void _add_sys_ext    (const Q *ext, Q *Y_N,         const int frame_id);
};
}
}

#endif /* CODEC_RSC_HPP_ */
