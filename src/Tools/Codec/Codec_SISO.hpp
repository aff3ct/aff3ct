#ifndef CODEC_SISO_HPP_
#define CODEC_SISO_HPP_

#include "Module/Decoder/SISO.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SISO : public Codec<B,Q>
{
public:
	Codec_SISO(const parameters& params)
	: Codec<B,Q>(params)
	{
	}

	virtual ~Codec_SISO()
	{
	}

	virtual module::SISO<Q>* build_siso(const int tid = 0, const module::Interleaver<int>* itl = nullptr,
	                                                             module::CRC        <B  >* crc = nullptr) = 0;
};
}
}

#endif /* CODEC_SISO_HPP_ */
