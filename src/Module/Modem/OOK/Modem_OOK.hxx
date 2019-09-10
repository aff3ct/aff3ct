#include "Module/Modem/OOK/Modem_OOK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q>
bool Modem_OOK<B,R,Q>
::is_complex_mod()
{
	return false;
}

template <typename B, typename R, typename Q>
bool Modem_OOK<B,R,Q>
::is_complex_fil()
{
	return false;
}

template <typename B, typename R, typename Q>
int Modem_OOK<B,R,Q>
::size_mod(const int N)
{
	return Modem<B,R,Q>::get_buffer_size_after_modulation(N, 1, 0, 1, is_complex_mod());
}

template <typename B, typename R, typename Q>
int Modem_OOK<B,R,Q>
::size_fil(const int N)
{
	return Modem<B,R,Q>::get_buffer_size_after_filtering(N, 1, 0, 1, is_complex_fil());
}
}
}
