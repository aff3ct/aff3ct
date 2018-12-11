#ifndef CONSTELLATION_PSK_HPP__
#define CONSTELLATION_PSK_HPP__

#include <vector>

#include "../Constellation_complex.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
class Constellation_PSK : public Constellation_complex<R>
{
public:
	using typename Constellation_complex<R>::S;

	/*
	 * \param n_bps is the number of bits per symbol
	 */
	explicit Constellation_PSK(const unsigned n_bps);

protected:
	S bits_to_symbol(const uint8_t bits[]) const;
};
}
}

#include "Constellation_PSK.hxx"

#endif // CONSTELLATION_PSK_HPP__