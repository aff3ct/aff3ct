#ifndef CODEC_TURBO_DB_HPP_
#define CODEC_TURBO_DB_HPP_

#include <vector>
#include <fstream>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"

#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

#include "Factory/Module/Code/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Code/RSC_DB/Decoder_RSC_DB.hpp"
#include "Factory/Module/Code/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Code/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Code/Turbo_DB/Puncturer_turbo_DB.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_turbo_DB : public Codec<B,Q>
{
protected:
	const factory::Encoder_turbo_DB  ::parameters &enc_par;
	const factory::Decoder_turbo_DB  ::parameters &dec_par;
	const factory::Puncturer_turbo_DB::parameters &pct_par;

	// the trellis representation
	std::vector<std::vector<int>>                               trellis;
	std::vector<module::Encoder_RSC_DB<B>*>                     sub_enc;
	std::vector<module::Decoder_RSC_DB_BCJR<B,Q>*>              siso_n;
	std::vector<module::Decoder_RSC_DB_BCJR<B,Q>*>              siso_i;
	std::vector<std::vector<tools::Post_processing_SISO<B,Q>*>> post_pros;

public:
	Codec_turbo_DB(const factory::Encoder_turbo_DB  ::parameters &enc_params,
	               const factory::Decoder_turbo_DB  ::parameters &dec_params,
	               const factory::Puncturer_turbo_DB::parameters &pct_params,
	               const int n_threads);
	virtual ~Codec_turbo_DB();

	tools ::Interleaver_core   <   >* build_interleaver(const int tid = 0, const int seed = 0                         );
	module::Puncturer          <B,Q>* build_puncturer  (const int tid = 0                                             );
	module::Encoder_RSC_DB     <B  >* build_sub_encoder(const int tid = 0                                             );
	module::Encoder            <B  >* build_encoder    (const int tid = 0, const module::Interleaver<B>* itl = nullptr);
	module::Decoder_RSC_DB_BCJR<B,Q>* build_sub_siso   (const int tid = 0                                             );
	module::Decoder_SIHO       <B,Q>* build_decoder    (const int tid = 0, const module::Interleaver<Q>* itl = nullptr,
	                                                                             module::CRC        <B>* crc = nullptr);

private:
	void clear_post_processing(const int tid = 0);
};
}
}

#endif /* CODEC_TURBO_DB_HPP_ */
