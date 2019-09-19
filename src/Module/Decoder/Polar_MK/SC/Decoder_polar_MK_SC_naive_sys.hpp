#ifndef DECODER_POLAR_MK_SC_NAIVE_SYS_
#define DECODER_POLAR_MK_SC_NAIVE_SYS_

#include <functional>
#include <vector>

#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/Decoder/Polar_MK/SC/Decoder_polar_MK_SC_naive.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
class Decoder_polar_MK_ASCL_naive_CA_sys;

template <typename B = int, typename R = float>
class Decoder_polar_MK_SC_naive_sys : public Decoder_polar_MK_SC_naive<B,R>
{
	friend Decoder_polar_MK_ASCL_naive_CA_sys<B,R>;

public:
	Decoder_polar_MK_SC_naive_sys(const int&               K,
	                              const int&               N,
	                              const tools::Polar_code& code,
	                              const std::vector<bool>& frozen_bits,
	                              const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
	                                                                            const std::vector<B> &bits)>>> &lambdas,
	                              const int                n_frames = 1);

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
