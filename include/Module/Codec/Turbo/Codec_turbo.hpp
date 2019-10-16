#ifndef CODEC_TURBO_HPP_
#define CODEC_TURBO_HPP_

#include <memory>
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
#include "Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_turbo : public Codec_SIHO<B,Q>
{
protected:
	std::vector<std::vector<int>>                                  trellis;
	std::shared_ptr<module::Encoder_RSC_sys<B>>                    sub_enc;
	std::shared_ptr<module::Decoder_SISO   <Q>>                    sub_dec;
	std::vector<std::shared_ptr<tools::Post_processing_SISO<B,Q>>> post_pros;
	std::ofstream                                                  json_stream;

public:
	Codec_turbo(const factory::Encoder_turbo  ::parameters<> &enc_params,
	            const factory::Decoder_turbo  ::parameters<> &dec_params,
	            const factory::Interleaver    ::parameters   &itl_params,
	            const factory::Puncturer_turbo::parameters   *pct_params = nullptr,
	            CRC<B>* crc = nullptr);
	virtual ~Codec_turbo();

protected:
	void add_post_pro(tools::Post_processing_SISO<B,Q>* p);
};
}
}

#endif /* CODEC_TURBO_HPP_ */
