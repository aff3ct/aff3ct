/*!
 * \file
 * \brief Class tools::Constellation_PAM.
 */
#ifndef CONSTELLATION_PAM_HPP__
#define CONSTELLATION_PAM_HPP__

#include <cstdint>

#include "Tools/Constellation/Constellation.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Constellation_PAM : public Constellation<R>
{
public:
	using typename Constellation<R>::S;

	/*
	 * \param n_bps is the number of bits per symbol
	 */
	explicit Constellation_PAM(const unsigned n_bps);

protected:
	S bits_to_symbol(const uint8_t bits[]) const;
	const R sqrt_es;
};

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Constellation/PAM/Constellation_PAM.hxx"
#endif

#endif // CONSTELLATION_PAM_HPP__