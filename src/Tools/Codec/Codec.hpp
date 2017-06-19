#ifndef CODEC_HPP_
#define CODEC_HPP_

#include "Tools/Exception/exception.hpp"
#include "Tools/params.h"

#include "Module/CRC/CRC.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec
{
protected:
	const tools::parameters& params;

public:
	Codec(const parameters& params)
	: params(params)
	{
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

	virtual module::Decoder<B,Q>* build_decoder(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                                     module::CRC        <B  >* crc = nullptr) = 0;
};
}
}

#endif /* CODEC_HPP_ */
