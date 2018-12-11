#ifndef CONSTELLATION_QAM_HPP__
#define CONSTELLATION_QAM_HPP__

#include <vector>

#include "../Constellation_complex.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
class Constellation_QAM : public Constellation_complex<R>
{
public:
	using typename Constellation_complex<R>::S;

	/*
	 * \param n_bps is the number of bits per symbol
	 */
	explicit Constellation_QAM(const unsigned n_bps);

protected:
	S bits_to_symbol(const uint8_t bits[]) const;
	const R sqrt_es;
};
}
}

#include "Constellation_QAM.hxx"

#endif // CONSTELLATION_QAM_HPP__