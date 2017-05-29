#ifndef FACTORY_DECODER_POLAR_GEN_HPP
#define FACTORY_DECODER_POLAR_GEN_HPP

#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Decoder/Decoder.hpp"
#include "Module/CRC/CRC.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_polar_gen : public Factory
{
	static module::Decoder<B,R>* build(const std::string      type,
	                                   const std::string      implem,
	                                   const int              K,
	                                   const int              N,
	                                   const mipp::vector<B> &frozen_bits,
	                                   const std::string      simd_strategy = "",
	                                   const bool             sys_encoding  = true,
	                                   const int              L             = 1,
	                                         module::CRC<B>  *crc           = nullptr,
	                                   const int              n_frames      = 1);

	static void get_frozen_bits(const std::string      implem,
	                            const int              N,
	                                  mipp::vector<B> &frozen_bits);

private:
	template <class API_polar>
	static module::Decoder<B,R>* _build(const std::string      type,
	                                    const std::string      implem,
	                                    const int              K,
	                                    const int              N,
	                                    const mipp::vector<B> &frozen_bits,
	                                    const int              L        = 1,
	                                          module::CRC<B>  *crc      = nullptr,
	                                    const int              n_frames = 1);
};
}
}

#endif /* FACTORY_DECODER_POLAR_GEN_HPP */
