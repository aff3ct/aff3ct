/*!
 * \file
 * \brief Class tools::Constellation_PSK.
 */
#ifndef CONSTELLATION_PSK_HPP__
#define CONSTELLATION_PSK_HPP__

#include <cstdint>

#include "Tools/Constellation/Constellation.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Constellation_PSK : public Constellation<R>
{
public:
	using typename Constellation<R>::S;

	/*
	 * \param n_bps is the number of bits per symbol
	 */
	explicit Constellation_PSK(const unsigned n_bps);

protected:
	S bits_to_symbol(const uint8_t bits[]) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Constellation/PSK/Constellation_PSK.hxx"
#endif

#endif // CONSTELLATION_PSK_HPP__