#ifndef CONSTELLATION_HXX__
#define CONSTELLATION_HXX__

#include <utility>
#include <cmath>
#include <iostream>

#include "Constellation.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"


namespace aff3ct
{
namespace tools
{
inline const std::string& Constellation::
get_name() const
{
	return name;
}

inline unsigned Constellation::
get_n_bits_per_symbol() const
{
	return n_bps;
}

inline unsigned Constellation::
get_n_symbols() const
{
	return n_symbs;
}






template <typename Symbol_type>
Constellation_S<Symbol_type>::
Constellation_S(const unsigned n_bps, const std::string& name)
: Constellation(n_bps, name),
  constellation(this->get_n_symbols())
{

}

template <typename Symbol_type>
Constellation_S<Symbol_type>::
Constellation_S(const std::vector<S>& symbols, const std::string& name)
: Constellation_S(std::move(std::vector<S>(symbols)), name)
{

}

template <typename Symbol_type>
Constellation_S<Symbol_type>::
Constellation_S(std::vector<S>&& symbols, const std::string& name)
: Constellation(std::log2(symbols.size()), name),
  constellation(std::move(symbols))
{
	if (constellation.size() != this->get_n_symbols())
	{
		std::stringstream message;
		message << "'symbols.size()' has to be a power of 2 ('symbols.size()' = "
		        << symbols.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename Symbol_type>
inline const Symbol_type Constellation_S<Symbol_type>::
operator[](const size_t idx) const
{
	return constellation[idx];
}

template <typename Symbol_type>
inline const Symbol_type Constellation_S<Symbol_type>::
get_symbol(const size_t idx) const
{
	return constellation[idx];
}

template <typename Symbol_type>
Symbol_type Constellation_S<Symbol_type>::
bits_to_symbol(const uint8_t bits[]) const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename Symbol_type>
void Constellation_S<Symbol_type>::
build()
{
	std::vector<uint8_t> bits(this->get_n_bits_per_symbol());

	for (unsigned j = 0; j < this->get_n_symbols(); j++)
	{
		for (unsigned l = 0; l < this->get_n_bits_per_symbol(); l++)
			bits[l] = (uint8_t)((j >> l) & 1);
		this->constellation[j] = this->bits_to_symbol(bits.data());
	}
}

}
}
#endif // CONSTELLATION_HXX__