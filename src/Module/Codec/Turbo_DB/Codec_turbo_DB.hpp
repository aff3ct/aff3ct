#ifndef CODEC_TURBO_DB_HPP_
#define CODEC_TURBO_DB_HPP_

#include <vector>
#include <memory>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"
#include "Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_turbo_DB : public Codec_SIHO<B,Q>
{
protected:
	std::vector<std::vector<int>>                                  trellis;
	std::shared_ptr<module::Encoder_RSC_DB<B>>                     sub_enc;
	std::shared_ptr<module::Decoder_RSC_DB_BCJR<B,Q>>              sub_dec_n;
	std::shared_ptr<module::Decoder_RSC_DB_BCJR<B,Q>>              sub_dec_i;
	std::vector<std::shared_ptr<tools::Post_processing_SISO<B,Q>>> post_pros;

public:
	Codec_turbo_DB(const factory::Encoder_turbo_DB  ::parameters &enc_params,
	               const factory::Decoder_turbo_DB  ::parameters &dec_params,
	               const factory::Interleaver       ::parameters &itl_params,
	               const factory::Puncturer_turbo_DB::parameters *pct_params = nullptr,
	               CRC<B>* crc = nullptr);
	virtual ~Codec_turbo_DB() = default;

protected:
	void add_post_pro(tools::Post_processing_SISO<B,Q>* p);
};
}
}

#endif /* CODEC_TURBO_DB_HPP_ */
