#ifndef CONSTELLATION_PSK_HXX__
#define CONSTELLATION_PSK_HXX__

#include <cmath>

#include "Tools/Math/math_constants.h"

#include "Tools/Constellation/PSK/Constellation_PSK.hpp"


namespace aff3ct
{
namespace tools
{

template <typename R>
Constellation_PSK<R>::
Constellation_PSK(const unsigned n_bps)
: Constellation<R>(n_bps, "PSK")
{
	this->build();
}

template <typename R>
inline typename Constellation_PSK<R>::S Constellation_PSK<R>::
bits_to_symbol(const uint8_t bits[]) const
{
	auto symbol = (R)1.0 - ((R)bits[0] + (R)bits[0]);
	for (unsigned j = 1; j < this->get_n_bits_per_symbol(); j++)
		symbol = ((R)1.0 - ((R)bits[j] + (R)bits[j])) * ((1 << j) - symbol);

	return std::complex<R>((R)std::cos((symbol +1) * (R)M_PI / this->get_n_symbols()),
	                       (R)std::sin((symbol +1) * (R)M_PI / this->get_n_symbols()));
}

}
}
#endif // CONSTELLATION_PSK_HXX__