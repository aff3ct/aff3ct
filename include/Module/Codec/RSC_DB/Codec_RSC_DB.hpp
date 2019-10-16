#ifndef CODEC_RSC_DB_HPP_
#define CODEC_RSC_DB_HPP_

#include <vector>

#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"
#include "Module/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_RSC_DB : public Codec_SISO_SIHO<B,Q>
{
protected:
	const bool buffered_encoding;
	std::vector<std::vector<int>> trellis;

public:
	Codec_RSC_DB(const factory::Encoder_RSC_DB::parameters &enc_params,
	             const factory::Decoder_RSC_DB::parameters &dec_params);
	virtual ~Codec_RSC_DB() = default;
};
}
}

#endif /* CODEC_RSC_DB_HPP_ */
