#ifndef CODEC_HPP_
#define CODEC_HPP_

#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/CRC/CRC.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

#include "Factory/Module/Code/Encoder.hpp"
#include "Factory/Module/Code/Decoder.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec
{
protected :
	const factory::Encoder::parameters &enc_params;
	const factory::Decoder::parameters &dec_params;

public:
	Codec(const factory::Encoder::parameters &enc_params,
	      const factory::Decoder::parameters &dec_params)
	: enc_params(enc_params), dec_params(dec_params)
	{
		if (enc_params.K != dec_params.K)
		{
			std::stringstream message;
			message << "The encoder dimension 'K' (=" << enc_params.K << ") is different than the decoder one's (="
			        << dec_params.K << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (enc_params.N_cw != dec_params.N_cw)
		{
			std::stringstream message;
			message << "The encoder dimension 'N_cw' (=" << enc_params.N_cw << ") is different than the decoder one's"
			           " (=" << dec_params.N_cw << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual ~Codec()
	{
	}

	virtual void launch_precompute()
	{
	}

	virtual void snr_precompute(const float sigma)
	{
	}

	virtual module::Interleaver<int>* build_interleaver(const int tid = 0, const int seed = 0)
	{
		throw cannot_allocate(__FILE__, __LINE__, __func__);
	}

	virtual module::Puncturer<B,Q>* build_puncturer(const int tid = 0)
	{
		throw cannot_allocate(__FILE__, __LINE__, __func__);
	}

	virtual module::Encoder<B>* build_encoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr)
	{
		throw cannot_allocate(__FILE__, __LINE__, __func__);
	}

	virtual module::Decoder_SIHO<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                          module::CRC        <B  >* crc = nullptr) = 0;
};
}
}

#endif /* CODEC_HPP_ */
