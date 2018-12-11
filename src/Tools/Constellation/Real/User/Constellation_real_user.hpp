#ifndef CONSTELLATION_REAL_USER_HPP__
#define CONSTELLATION_REAL_USER_HPP__

#include <vector>

#include "../Constellation_real.hpp"
#include "../../Complex/Constellation_complex.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
class Constellation_real_user : public Constellation_real<R>
{
public:
	using typename Constellation_real<R>::S;

	/*
	 * \param const_path is the constellation file path from which the constellation is read
	 */
	explicit Constellation_real_user(const std::string& const_path);


	/*
	 * \param cstl is a complex constellation whom real part is copied
	 */
	explicit Constellation_real_user(const Constellation_complex<R>& cstl);
};

}
}

#include "Constellation_real_user.hxx"

#endif // CONSTELLATION_REAL_USER_HPP__