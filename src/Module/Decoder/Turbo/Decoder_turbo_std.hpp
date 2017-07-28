#ifndef DECODER_TURBO_NAIVE_HPP
#define DECODER_TURBO_NAIVE_HPP

#include <vector>
#include <mipp.h>

#include "Module/Interleaver/Interleaver.hpp"

#include "Decoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_std : public Decoder_turbo<B,R>
{
public:
	Decoder_turbo_std(const int& K,
	                  const int& N,
	                  const int& n_ite,
	                  const Interleaver<int> &pi,
	                  SISO<R> &siso_n,
	                  SISO<R> &siso_i,
	                  const bool buffered_encoding = true,
	                  const std::string name = "Decoder_turbo_std");
	virtual ~Decoder_turbo_std();

protected:
	virtual void _hard_decode(const R *Y_N, B *V_K, const int frame_id);
};
}
}

#endif /* DECODER_TURBO_NAIVE_HPP */
