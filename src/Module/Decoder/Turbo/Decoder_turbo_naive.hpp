#ifndef DECODER_TURBO_NAIVE_HPP
#define DECODER_TURBO_NAIVE_HPP

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../Interleaver/Interleaver.hpp"

#include "Decoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_turbo_naive : public Decoder_turbo<B,R>
{
public:
	Decoder_turbo_naive(const int& K,
	                    const int& N_without_tb,
	                    const int& n_ite,
	                    const Interleaver<short> &pi,
	                    SISO<R> &siso_n,
	                    SISO<R> &siso_i,
	                    tools::Scaling_factor<R> &scaling_factor,
	                    const bool buffered_encoding = true,
	                    const std::string name = "Decoder_turbo_naive");
	virtual ~Decoder_turbo_naive();

protected:
	virtual void hard_decode();
};
}
}

#endif /* DECODER_TURBO_NAIVE_HPP */
