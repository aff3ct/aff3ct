#ifndef DECODER_TURBO_FAST_SYS_
#define DECODER_TURBO_FAST_SYS_

#include <vector>
#include "Tools/MIPP/mipp.h"

#include "../../Interleaver/Interleaver.hpp"

#include "Decoder_turbo.hpp"

template <typename B, typename R>
class Decoder_turbo_fast : public Decoder_turbo<B,R>
{
public:
	Decoder_turbo_fast(const int& K,
	                   const int& N_without_tb,
	                   const int& n_ite,
	                   const Interleaver<short> &pi,
	                   SISO<R> &siso_n,
	                   SISO<R> &siso_i,
	                   Scaling_factor<R> &scaling_factor,
	                   const bool buffered_encoding = true,
	                   const std::string name = "Decoder_turbo_fast");
	virtual ~Decoder_turbo_fast();

	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;
};

#include "Decoder_turbo_fast.hxx"

#endif /* DECODER_TURBO_FAST_SYS_ */
