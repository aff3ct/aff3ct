/*!
 * \file
 * \brief Functions for SCMA modem.
 */
#ifndef MODEM_SCMA_FUNCTIONS_HPP
#define MODEM_SCMA_FUNCTIONS_HPP

#include <complex>

namespace aff3ct
{
namespace tools
{
// -------------------------------------------------------------------------- special function prototypes for templates
template <typename R>
using proto_psi = R (*)(const std::complex<R>& d, const R& n0);

// ------------------------------------------------------------------------------------------- special function headers

template <typename R>
inline R psi_0(const std::complex<R>& d, const R& n0);

template <typename R>
inline R psi_1(const std::complex<R>& d, const R& n0);

template <typename R>
inline R psi_2(const std::complex<R>& d, const R& n0);

template <typename R>
inline R psi_3(const std::complex<R>& d, const R& n0);

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/SCMA/modem_SCMA_functions.hxx"
#endif

#endif /* MODEM_SCMA_FUNCTIONS_HPP */
