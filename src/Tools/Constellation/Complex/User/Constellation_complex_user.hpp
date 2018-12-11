#ifndef CONSTELLATION_COMPLEX_USER_HPP__
#define CONSTELLATION_COMPLEX_USER_HPP__

#include <vector>

#include "../Constellation_complex.hpp"
#include "../../Complex/Constellation_complex.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
class Constellation_complex_user : public Constellation_complex<R>
{
public:
	using typename Constellation_complex<R>::S;

	/*
	 * \param n_bps is the number of bits per symbol
	 */
	explicit Constellation_complex_user(const std::string& const_path);

private:
	static std::vector<S> read_constellation(const std::string& const_path);
};

}
}

#include "Constellation_complex_user.hxx"

#endif // CONSTELLATION_COMPLEX_USER_HPP__