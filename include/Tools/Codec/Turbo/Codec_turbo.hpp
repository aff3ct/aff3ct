/*!
 * \file
 * \brief Class tools::Codec_turbo.
 */
#ifndef CODEC_TURBO_HPP_
#define CODEC_TURBO_HPP_

#include <vector>
#include <fstream>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Factory/Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Factory/Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Puncturer/Turbo/Puncturer_turbo.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_turbo : public Codec_SIHO<B,Q>
{
protected:
	std::vector<std::vector<int>> trellis;
	std::shared_ptr<std::ofstream> json_stream;

public:
	Codec_turbo(const factory::Encoder_turbo<> &enc_params,
	            const factory::Decoder_turbo<> &dec_params,
	            const factory::Interleaver     &itl_params,
	            const factory::Puncturer_turbo *pct_params = nullptr,
	            module::CRC<B>* crc = nullptr);
	virtual ~Codec_turbo();

	virtual Codec_turbo<B,Q>* clone() const;

	const std::vector<std::vector<int>>& get_trellis() const;
};
}
}

#endif /* CODEC_TURBO_HPP_ */
