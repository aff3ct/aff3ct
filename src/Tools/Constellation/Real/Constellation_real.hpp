#ifndef CONSTELLATION_REAL_HPP__
#define CONSTELLATION_REAL_HPP__

#include "../Constellation.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
using Constellation_real = Constellation_S<R>;

template <typename R>
bool is_constellation_real(const Constellation& cstl)
{
	try
	{
		volatile auto c = dynamic_cast<const Constellation_real<R>&>(cstl);
		return true;
	}
	catch(std::exception&)
	{
		return false;
	}
}

}
}

#endif // CONSTELLATION_REAL_HPP__