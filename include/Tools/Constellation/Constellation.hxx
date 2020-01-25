#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <utility>
#include <sstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/Constellation/Constellation.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
bool has_complex_symbols(const Constellation<R>& cstl)
{
	for (unsigned i = 0; i < cstl.get_n_symbols(); i++)
	{
		if (cstl[i].imag() != (R)0.)
			return true;
	}

	return false;
}

template <typename R>
Constellation<R>
::Constellation(std::vector<S>&& symbols, const std::string& name)
: n_bps((unsigned int)std::log2(symbols.size())),
  n_symbs(1 << n_bps),
  name(std::to_string(n_symbs) + name),
  constellation(std::move(symbols)),
  is_cplx(true)
{
	if (constellation.size() != this->get_n_symbols())
	{
		std::stringstream message;
		message << "'symbols.size()' has to be a power of 2 ('symbols.size()' = "
		        << symbols.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename R>
Constellation<R>
::Constellation(const unsigned n_bps, const std::string& name)
: Constellation(std::move(std::vector<S>((size_t)((int64_t)1 << n_bps))), name)
{
}

template <typename R>
Constellation<R>
::Constellation(const std::vector<S>& symbols, const std::string& name)
: Constellation(std::move(std::vector<S>(symbols)), name)
{
}

template <typename R>
const std::string& Constellation<R>
::get_name() const
{
	return name;
}

template <typename R>
unsigned Constellation<R>
::get_n_bits_per_symbol() const
{
	return n_bps;
}

template <typename R>
unsigned Constellation<R>
::get_n_symbols() const
{
	return n_symbs;
}

template <typename R>
bool Constellation<R>
::is_complex() const
{
	return is_cplx;
}

template <typename R>
const typename Constellation<R>::S& Constellation<R>
::operator[](const size_t idx) const
{
	return constellation[idx];
}

template <typename R>
const typename Constellation<R>::S& Constellation<R>
::get_symbol(const size_t idx) const
{
	return constellation[idx];
}

template <typename R>
R Constellation<R>
::get_imag(const size_t idx) const
{
	return constellation[idx].imag();
}

template <typename R>
R Constellation<R>
::get_real(const size_t idx) const
{
	return constellation[idx].real();
}

template <typename R>
size_t Constellation<R>
::size() const
{
	return constellation.size();
}

template <typename R>
typename Constellation<R>::S Constellation<R>
::bits_to_symbol(const uint8_t bits[]) const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R>
void Constellation<R>
::build()
{
	try
	{
		std::vector<uint8_t> bits(this->get_n_bits_per_symbol());

		for (unsigned j = 0; j < this->get_n_symbols(); j++)
		{
			for (unsigned l = 0; l < this->get_n_bits_per_symbol(); l++)
				bits[l] = (uint8_t)((j >> l) & 1);
			this->constellation[j] = this->bits_to_symbol(bits.data());
		}
	}
	catch(tools::unimplemented_error&)
	{} // constellation has been filled by another way

	is_cplx = has_complex_symbols(*this);
}

}
}
