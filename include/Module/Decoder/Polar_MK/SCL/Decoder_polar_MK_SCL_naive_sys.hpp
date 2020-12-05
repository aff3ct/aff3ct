/*!
 * \file
 * \brief Class module::Decoder_polar_MK_SCL_naive_sys.
 */
#ifndef DECODER_POLAR_MK_SCL_NAIVE_SYS
#define DECODER_POLAR_MK_SCL_NAIVE_SYS

#include <functional>
#include <vector>

#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/Decoder/Polar_MK/SCL/Decoder_polar_MK_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_polar_MK_SCL_naive_sys : public Decoder_polar_MK_SCL_naive<B,R>
{

public:
	Decoder_polar_MK_SCL_naive_sys(const int&               K,
	                               const int&               N,
	                               const int&               L,
	                               const tools::Polar_code& code,
	                               const std::vector<bool>& frozen_bits,
	                               const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
	                                                                             const std::vector<B> &bits)>>> &lambdas);

	Decoder_polar_MK_SCL_naive_sys(const int&               K,
	                               const int&               N,
	                               const int&               L,
	                               const tools::Polar_code& code,
	                               const std::vector<bool>& frozen_bits);

	virtual ~Decoder_polar_MK_SCL_naive_sys() = default;

	virtual Decoder_polar_MK_SCL_naive_sys<B,R>* clone() const;

protected:
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_SYS */
