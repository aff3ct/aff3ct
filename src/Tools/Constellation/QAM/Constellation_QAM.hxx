#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "Tools/Constellation/QAM/Constellation_QAM.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Constellation_QAM<R>
::Constellation_QAM(const unsigned n_bps)
: Constellation<R>(n_bps, "QAM"),
  sqrt_es((R)std::sqrt((R)2.0 * (R)(this->get_n_symbols() - 1) / 3.0))
{
	this->build();
}

template <typename R>
typename Constellation_QAM<R>::S Constellation_QAM<R>
::bits_to_symbol(const uint8_t bits[]) const
{
	auto bps = this->get_n_bits_per_symbol();

	auto symbol_I = (R)1.0 - (bits[      0] + bits[      0]);
	auto symbol_Q = (R)1.0 - (bits[bps / 2] + bits[bps / 2]);

	for (unsigned j = 1; j < bps / 2; j++)
	{
		symbol_I = ((R)1.0 - (bits[         j] + bits[         j])) * ((1 << j) - symbol_I);
		symbol_Q = ((R)1.0 - (bits[bps / 2 +j] + bits[bps / 2 +j])) * ((1 << j) - symbol_Q);
	}

	return S(symbol_I, symbol_Q) / S(this->sqrt_es);
}

}
}
