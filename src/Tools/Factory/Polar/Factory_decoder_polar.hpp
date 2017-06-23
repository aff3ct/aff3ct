#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include <string>
#include <mipp.h>

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Decoder.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_polar : public Factory
{
	static module::Decoder_SISO<B,R>* build_siso(const std::string      type,
	                                             const std::string      implem,
	                                             const int              K,
	                                             const int              N,
	                                             const mipp::vector<B> &frozen_bits,
	                                             const bool             sys_encoding = true,
	                                             const int              n_ite        = 4,
	                                             const int              n_frames     = 1);

	static module::Decoder<B,R>* build(const std::string      type,
	                                   const std::string      implem,
	                                   const int              K,
	                                   const int              N,
	                                   const mipp::vector<B> &frozen_bits,
	                                   const std::string      simd_strategy = "",
	                                   const bool             sys_encoding  = true,
	                                   const std::string      polar_nodes   = "{R0,R0L,R1,REP,REPL,SCL_4}",
	                                   const int              L             = 1,
	                                   const int              n_ite         = 4,
	                                         module::CRC<B>  *crc           = nullptr,
	                                   const bool             full_adaptive = true,
	                                   const int              n_frames      = 1);

private:
	template <class API_polar>
	static module::Decoder<B,R>* _build(const std::string      type,
	                                    const std::string      implem,
	                                    const int              K,
	                                    const int              N,
	                                    const mipp::vector<B> &frozen_bits,
	                                    const bool             sys_encoding  = true,
	                                    const std::string      polar_nodes   = "{R0,R0L,R1,REP,REPL,SCL_4}",
	                                    const int              L             = 1,
	                                    const int              n_ite         = 4,
	                                          module::CRC<B>  *crc           = nullptr,
	                                    const bool             full_adaptive = true,
	                                    const int              n_frames      = 1);

	template <class API_polar>
	static module::Decoder<B,R>* _build_scl_fast(const std::string      type,
	                                             const std::string      implem,
	                                             const int              K,
	                                             const int              N,
	                                             const mipp::vector<B> &frozen_bits,
	                                             const bool             sys_encoding  = true,
	                                             const std::string      polar_nodes   = "{R0,R0L,R1,REP,REPL,SCL_4}",
	                                             const int              L             = 1,
	                                                   module::CRC<B>  *crc           = nullptr,
	                                             const bool             full_adaptive = true,
	                                             const int              n_frames      = 1);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
