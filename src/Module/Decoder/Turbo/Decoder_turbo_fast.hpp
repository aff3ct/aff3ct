#ifndef DECODER_TURBO_FAST_SYS_
#define DECODER_TURBO_FAST_SYS_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../../Interleaver/Interleaver.hpp"

#include "Decoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_fast : public Decoder_turbo<B,R>
{
public:
	Decoder_turbo_fast(const int& K,
	                   const int& N,
	                   const int& n_ite,
	                   const Interleaver<int> &pi,
	                   SISO<R> &siso_n,
	                   SISO<R> &siso_i,
	                   tools::Scaling_factor<R> &scaling_factor,
	                   const bool buffered_encoding = true,
	                   const std::string name = "Decoder_turbo_fast");
	virtual ~Decoder_turbo_fast();

protected:
	void _load       (const R *Y_N        );
	void _hard_decode(const R *Y_N, B *V_K);
	void _store      (              B *V_K) const;
};
}
}

#endif /* DECODER_TURBO_FAST_SYS_ */
