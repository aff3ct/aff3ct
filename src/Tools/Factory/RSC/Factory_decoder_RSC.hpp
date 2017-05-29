#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float, typename RD = R>
struct Factory_decoder_RSC : public Factory
{
	static module::Decoder_SISO<B,R>* build(const std::string                    type,
	                                        const std::string                    implem,
	                                        const int                            K,
	                                        const std::vector<std::vector<int>> &trellis,
	                                        const std::string                    max_type      = "MAX",
	                                        const std::string                    simd_strategy = "",
	                                        const bool                           buffered      = true,
	                                              std::ostream                  &stream        = std::cout,
	                                        const int                            n_ite         = 1,
	                                        const int                            n_frames      = 1);

private:
	template <proto_max<R> MAX1, proto_max<RD> MAX2>
	static module::Decoder_SISO<B,R>* _build_seq(const std::string                    type,
	                                             const std::string                    implem,
	                                             const int                            K,
	                                             const std::vector<std::vector<int>> &trellis,
	                                             const bool                           buffered = true,
	                                                   std::ostream                  &stream   = std::cout,
	                                             const int                            n_ite    = 1,
	                                             const int                            n_frames = 1);

	template <proto_max_i<R> MAX>
	static module::Decoder_SISO<B,R>* _build_simd(const std::string                    type,
	                                              const std::string                    implem,
	                                              const int                            K,
	                                              const std::vector<std::vector<int>> &trellis,
	                                              const std::string                    simd_strategy = "INTRA",
	                                              const bool                           buffered      = true,
	                                              const int                            n_frames      = 1);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
