#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <string>
#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_LDPC : public Factory
{
	static module::Decoder_SISO<B,R>* build(const std::string            type,
	                                        const std::string            implem,
	                                        const int                    K,
	                                        const int                    N,
	                                        const int                    n_ite,
	                                        const Sparse_matrix         &H,
	                                        const std::vector<unsigned> &info_bits_pos,
	                                        const std::string            simd_strategy = "",
	                                        const float                  factor        = 1.f,
	                                        const R                      offset        = 0,
	                                        const bool                   synd          = true,
	                                        const int                    synd_depth    = 2,
	                                        const int                    n_frames      = 1);
};
}
}

#endif /* FACTORY_DECODER_LDPC_HPP */
