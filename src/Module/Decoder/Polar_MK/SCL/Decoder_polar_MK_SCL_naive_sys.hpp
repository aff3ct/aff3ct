#ifndef DECODER_POLAR_MK_SCL_NAIVE_SYS
#define DECODER_POLAR_MK_SCL_NAIVE_SYS

#include <vector>

#include "Decoder_polar_MK_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_polar_MK_SCL_naive_sys : public Decoder_polar_MK_SCL_naive<B,R>
{

public:
	Decoder_polar_MK_SCL_naive_sys(const int&               K,
	                               const int&               N,
	                               const int&               L,
	                               const tools::Polar_code& code,
	                               const std::vector<bool>& frozen_bits,
	                               const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
	                                                                             const std::vector<B> &bits)>>> &lambdas,
	                               const int                n_frames = 1);

	Decoder_polar_MK_SCL_naive_sys(const int&               K,
	                               const int&               N,
	                               const int&               L,
	                               const tools::Polar_code& code,
	                               const std::vector<bool>& frozen_bits,
	                               const int                n_frames = 1);

	virtual ~Decoder_polar_MK_SCL_naive_sys() = default;

protected:
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_SYS */
