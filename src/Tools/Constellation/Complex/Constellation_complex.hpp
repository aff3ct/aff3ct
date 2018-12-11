#ifndef CONSTELLATION_COMPLEX_HPP__
#define CONSTELLATION_COMPLEX_HPP__

#include <complex>

#include "../Constellation.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
using Constellation_complex = Constellation_S<std::complex<R>>;

template <typename R>
bool is_constellation_complex(const Constellation& cstl)
{
	try
	{
		volatile auto c = dynamic_cast<const Constellation_complex<R>&>(cstl);
		return true;
	}
	catch(std::exception&)
	{
		return false;
	}
}

template <typename R>
bool has_real_symbols(const Constellation_complex<R>& cstl)
{
	for (unsigned i = 0; i < cstl.get_n_symbols(); i++)
	{
		if (cstl[i].imag() != (R)0.)
			return false;
	}

	return true;
}

}
}
#endif // CONSTELLATION_COMPLEX_HPP__