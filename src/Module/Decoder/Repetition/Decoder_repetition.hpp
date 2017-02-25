#ifndef DECODER_REPETITION
#define DECODER_REPETITION

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Decoder.hpp"
#include "../SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_repetition : public Decoder<B,R>, public SISO<R>
{
protected:
	const int rep_count; // number of repetitions

	const bool buffered_encoding;

	mipp::vector<R> sys;
	mipp::vector<R> par;
	mipp::vector<R> ext;
	mipp::vector<B> s;

public:
	Decoder_repetition(const int& K, const int& N, const bool buffered_encoding = true, 
	                   const int n_frames = 1, const std::string name = "Decoder_repetition");
	virtual ~Decoder_repetition();

	virtual void soft_decode (const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;

protected:
	void load        (const mipp::vector<R>& Y_N                        );
	void hard_decode (                                                  );
	void store       (      mipp::vector<B>& V_K                        ) const;
	void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);
};
}
}

#endif /* DECODER_REPETITION */
