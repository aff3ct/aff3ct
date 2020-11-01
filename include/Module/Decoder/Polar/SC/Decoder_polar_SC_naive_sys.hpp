/*!
 * \file
 * \brief Class module::Decoder_polar_SC_naive_sys.
 */
#ifndef DECODER_POLAR_SC_NAIVE_SYS_
#define DECODER_POLAR_SC_NAIVE_SYS_

#include <vector>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_f<  R> F = tools::f_LLR,
                                                tools::proto_g<B,R> G = tools::g_LLR,
                                                tools::proto_h<B,R> H = tools::h_LLR>
class Decoder_polar_SC_naive_sys : public Decoder_polar_SC_naive<B,R,F,G,H>
{
public:
	Decoder_polar_SC_naive_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits);
	virtual ~Decoder_polar_SC_naive_sys() = default;

	virtual Decoder_polar_SC_naive_sys<B,R,F,G,H>* clone() const;

protected:
	void _store(B *V, bool coded = false) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive_sys.hxx"
#endif

#endif /* DECODER_POLAR_SC_NAIVE_SYS_ */
