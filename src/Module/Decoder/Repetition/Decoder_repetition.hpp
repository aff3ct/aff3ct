#ifndef DECODER_REPETITION
#define DECODER_REPETITION

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Decoder.hpp"
#include "../SISO.hpp"

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
	                   const std::string name = "Decoder_repetition");
	virtual ~Decoder_repetition();

	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;

	void set_n_frames(const int n_frames);

	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;
	        void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);
};

#include "Decoder_repetition.hxx"

#endif /* DECODER_REPETITION */
