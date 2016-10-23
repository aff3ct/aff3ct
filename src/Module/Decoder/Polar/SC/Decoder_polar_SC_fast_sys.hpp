#ifndef DECODER_POLAR_SC_FAST_SYS_
#define DECODER_POLAR_SC_FAST_SYS_

#include <map>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../decoder_polar_functions.h"
#include "../../Decoder.hpp"

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys : public Decoder<B,R>
{
protected:
	const int             m;            // graph depth
	      mipp::vector<R> l;            // lambda, LR or LLR
	      mipp::vector<B> s;            // bits, partial sums
	      mipp::vector<B> s_bis;        // bits, partial sums
	const mipp::vector<B> &frozen_bits; // frozen bits

	std::vector<char> pattern_types_per_id;

public:
	Decoder_polar_SC_fast_sys(const int& K, const int& N, const mipp::vector<B>& frozen_bits, 
	                          const int n_frames = 1, const std::string name = "Decoder_polar_SC_fast_sys");
	virtual ~Decoder_polar_SC_fast_sys();

	        void load       (const mipp::vector<R>& Y_N);
	virtual void hard_decode(                          );
	        void store      (      mipp::vector<B>& V_K) const;
	        void store_fast (      mipp::vector<B>& V_N) const;
	        void unpack     (      mipp::vector<B>& V_N) const;

protected:
	virtual void recursive_decode(const int off_l, const int off_s, const int reverse_depth, int &id);
};

#include "Decoder_polar_SC_fast_sys.hxx"

#endif /* DECODER_POLAR_SC_FAST_SYS_ */
