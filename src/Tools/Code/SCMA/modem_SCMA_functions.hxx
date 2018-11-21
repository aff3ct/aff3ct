#ifndef MODEM_SCMA_FUNCTIONS_HXX_
#define MODEM_SCMA_FUNCTIONS_HXX_

#include <cmath>

#include "modem_SCMA_functions.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
inline R psi_0(const std::complex<R>& d, const R& n0)
{
	return (R)std::exp(-std::norm(d) / n0);
}

template <typename R>
inline R psi_1(const std::complex<R>& d, const R& n0)
{
	return 1 / (std::norm(d) + n0);
}

template <typename R>
inline R psi_2(const std::complex<R>& d, const R& n0)
{
	return (R)((R)1 / (8 * std::pow(std::norm(d),2) + n0));
}

template <typename R>
inline R psi_3(const std::complex<R>& d, const R& n0)
{
	return (R)((R)1 / (4 * std::pow(std::norm(d),2) + n0));
}
}
}

#endif // MODEM_SCMA_FUNCTIONS_HXX_
