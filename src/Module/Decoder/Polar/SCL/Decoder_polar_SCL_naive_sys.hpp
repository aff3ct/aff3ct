#ifndef DECODER_POLAR_SCL_NAIVE_SYS
#define DECODER_POLAR_SCL_NAIVE_SYS

#include <vector>
#include <set>
#include "../../Decoder.hpp"
#include "../decoder_polar_functions.h"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
class Decoder_polar_SCL_naive_sys : public Decoder_polar_SCL_naive<B,R,F,G>
{

public:
	Decoder_polar_SCL_naive_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits,
	                            const int n_frames = 1, const std::string name = "Decoder_polar_SCL_naive_sys");

	virtual ~Decoder_polar_SCL_naive_sys() {}

protected:
	virtual void store(mipp::vector<B>& V_K) const;
};
}
}

#include "Decoder_polar_SCL_naive_sys.hxx"

#endif /* DECODER_POLAR_SCL_NAIVE_SYS */
