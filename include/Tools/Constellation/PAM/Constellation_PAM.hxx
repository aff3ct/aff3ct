#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "Tools/Constellation/PAM/Constellation_PAM.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
Constellation_PAM<R>
::Constellation_PAM(const unsigned n_bps)
: Constellation<R>(n_bps, "PAM"),
  sqrt_es((R)std::sqrt(((R)(this->get_n_symbols() * this->get_n_symbols()) - 1.0) / 3.0))
{
	this->build();
}

template <typename R>
typename Constellation_PAM<R>::S Constellation_PAM<R>
::bits_to_symbol(const uint8_t bits[]) const
{
	auto symbol = (R)1.0 - ((R)bits[0] + (R)bits[0]);
	for (unsigned j = 1; j < this->get_n_bits_per_symbol(); j++)
		symbol = ((R)1.0 - ((R)bits[j] + (R)bits[j])) * ((1 << j) - symbol);

	return symbol / this->sqrt_es;
}

}
}
