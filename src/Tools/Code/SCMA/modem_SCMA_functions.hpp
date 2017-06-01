#ifndef MODEM_SCMA_FUNCTIONS_HPP
#define MODEM_SCMA_FUNCTIONS_HPP

#include <complex>

#ifndef _MSC_VER
#ifndef __forceinline
#define __forceinline inline __attribute__((always_inline))
#endif
#endif

namespace aff3ct
{
namespace tools
{
// -------------------------------------------------------------------------- special function prototypes for templates
template <typename R>
using proto_psi = R (*)(const std::complex<R>& d, const R& n0);

// ------------------------------------------------------------------------------------------- special function headers

template <typename R>
__forceinline R psi_0(const std::complex<R>& d, const R& n0);

template <typename R>
__forceinline R psi_1(const std::complex<R>& d, const R& n0);

template <typename R>
__forceinline R psi_2(const std::complex<R>& d, const R& n0);

template <typename R>
__forceinline R psi_3(const std::complex<R>& d, const R& n0);

}
}

#include "modem_SCMA_functions.hxx"

#endif /* MODEM_SCMA_FUNCTIONS_HPP */
