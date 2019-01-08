#ifndef CONSTELLATION_PAM_HPP__
#define CONSTELLATION_PAM_HPP__

#include <vector>

#include "../Constellation.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
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

#include "Constellation_PAM.hxx"

#endif // CONSTELLATION_PAM_HPP__