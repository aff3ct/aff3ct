#ifndef CODEC_SISO_HPP_
#define CODEC_SISO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SISO : public Codec<B,Q>
{
public:
	Codec_SISO(const factory::Encoder::parameters &enc_params, const factory::Decoder::parameters &dec_params)
	: Codec<B,Q>(enc_params, dec_params)
	{
	}

	virtual ~Codec_SISO()
	{
	}

	virtual module::Decoder_SISO<Q>* build_siso(const int tid = 0, const module::Interleaver<Q>* itl = nullptr,
	                                                                     module::CRC        <B>* crc = nullptr) = 0;

	virtual void extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par) = 0;
};
}
}

#endif /* CODEC_SISO_HPP_ */
