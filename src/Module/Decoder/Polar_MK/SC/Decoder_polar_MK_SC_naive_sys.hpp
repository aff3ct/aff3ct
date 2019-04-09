#ifndef DECODER_POLAR_MK_SC_NAIVE_SYS_
#define DECODER_POLAR_MK_SC_NAIVE_SYS_

#include <vector>

#include "Decoder_polar_MK_SC_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_polar_MK_SC_naive_sys : public Decoder_polar_MK_SC_naive<B,R>
{
public:
	Decoder_polar_MK_SC_naive_sys(const int&               K,
	                              const int&               N,
	                              const tools::Polar_code& code,
	                              const std::vector<bool>& frozen_bits,
	                              const int                n_frames = 1);
	virtual ~Decoder_polar_MK_SC_naive_sys() = default;

protected:
	void _store(B *V, bool coded = false) const;
};
}
}

#endif /* DECODER_POLAR_MK_SC_NAIVE_SYS_ */
