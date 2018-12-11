#ifndef CONSTELLATION_REAL_USER_HXX__
#define CONSTELLATION_REAL_USER_HXX__

#include <fstream>
#include <cmath>
#include <iterator>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Constellation_real_user.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R>
Constellation_real_user<R>::
Constellation_real_user(const std::string& const_path)
: Constellation_real<R>(Constellation_complex<R>(const_path))
{
}

template <typename R>
Constellation_real_user<R>::
Constellation_real_user(const Constellation_complex<R>& cstl)
: Constellation_real<R>(cstl.get_n_bits_per_symbol(), "User<R>")
{
	for (unsigned i = 0; i < this->get_n_symbols(); i++)
		this->constellation[i] = cstl[i].real();
}

}
}
#endif // CONSTELLATION_REAL_USER_HXX__