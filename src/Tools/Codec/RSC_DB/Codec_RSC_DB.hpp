#ifndef CODEC_RSC_DB_HPP_
#define CODEC_RSC_DB_HPP_

#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

#include "Factory/Module/Code/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Code/RSC_DB/Decoder_RSC_DB.hpp"

#include "../Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_RSC_DB : public Codec_SISO<B,Q>
{
protected:
	const factory::Encoder_RSC_DB::parameters& enc_par;
	const factory::Decoder_RSC_DB::parameters& dec_par;

	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Codec_RSC_DB(const factory::Encoder_RSC_DB::parameters &enc_params,
	             const factory::Decoder_RSC_DB::parameters &dec_params);
	virtual ~Codec_RSC_DB();

	module::Encoder_RSC_DB<B  >* build_encoder     (const int tid = 0, const module::Interleaver<int>* itl = nullptr);
	module::Decoder_SISO  <B,Q>* build_decoder_siso(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                         module::CRC        <B  >* crc = nullptr);
	module::SISO          <  Q>* build_siso        (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                         module::CRC        <B  >* crc = nullptr);
	module::Decoder       <B,Q>* build_decoder     (const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                         module::CRC        <B  >* crc = nullptr);

	void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par);
};
}
}

#endif /* CODEC_RSC_DB_HPP_ */
