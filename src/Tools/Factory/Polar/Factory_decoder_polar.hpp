#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Decoder/Decoder.hpp"
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename R>
struct Factory_decoder_polar : public Factory
{
	static module::SISO<R>* build_siso(const parameters &params, const mipp::vector<B> &frozen_bits);

	static module::Decoder<B,R>* build(const parameters &params, const mipp::vector<B> &frozen_bits,
	                                   module::CRC<B> *crc);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
