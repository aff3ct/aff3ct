#ifndef CODEC_TURBO_HPP_
#define CODEC_TURBO_HPP_

#include <vector>
#include <fstream>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"

#include "Factory/Module/Code/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Code/RSC/Decoder_RSC.hpp"
#include "Factory/Module/Code/Turbo/Encoder_turbo.hpp"
#include "Factory/Module/Code/Turbo/Decoder_turbo.hpp"
#include "Factory/Module/Code/Turbo/Puncturer_turbo.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float, typename QD = Q>
class Codec_turbo : public Codec<B,Q>
{
protected:
	const factory::Encoder_turbo  ::parameters<> &enc_par;
	const factory::Decoder_turbo  ::parameters<> &dec_par;
	const factory::Puncturer_turbo::parameters   &pct_par;

	// the trellis representation
	std::vector<std::vector<int>>                               trellis;
	std::vector<module::Encoder_RSC_sys<B>*>                    sub_enc;
	std::vector<module::SISO<Q>*>                               siso;
	std::vector<std::vector<tools::Post_processing_SISO<B,Q>*>> post_pros;
	std::ofstream                                               json_stream;

public:
	Codec_turbo(const factory::Encoder_turbo  ::parameters<> &enc_params,
	            const factory::Decoder_turbo  ::parameters<> &dec_params,
	            const factory::Puncturer_turbo::parameters   &pct_params,
	            const int n_threads);
	virtual ~Codec_turbo();

	module::Interleaver    <int>* build_interleaver(const int tid = 0, const int seed = 0                           );
	module::Puncturer      <B,Q>* build_puncturer  (const int tid = 0                                               );
	module::Encoder_RSC_sys<B  >* build_sub_encoder(const int tid = 0                                               );
	module::Encoder        <B  >* build_encoder    (const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::SISO           <  Q>* build_sub_siso   (const int tid = 0                                               );
	module::Decoder        <B,Q>* build_decoder    (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                         module::CRC        <B  >* crc = nullptr);

private:
	void clear_post_processing(const int tid = 0);
};
}
}

#endif /* CODEC_TURBO_HPP_ */
