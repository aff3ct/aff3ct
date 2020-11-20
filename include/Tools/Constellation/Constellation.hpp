/*!
 * \file
 * \brief Class tools::Constellation.
 */
#ifndef CONSTELLATION_HPP__
#define CONSTELLATION_HPP__

#include <cstdint>
#include <cstddef>
#include <complex>
#include <string>
#include <vector>

namespace aff3ct
{
namespace tools
{
/*
 * \brief describe a constellation
 * \param Symbol_type is the symbol type as a real or a complex
 */
template <typename R = float>
class Constellation
{
public:
	using S = std::complex<R>;

	virtual ~Constellation() = default;

	/*
	 * \brief get the name of the constellation
	 */
	const std::string& get_name() const;

	/*
	 * \brief get the number of bits per symbol
	 */
	unsigned get_n_bits_per_symbol() const;

	/*
	 * \brief get the number of symbols in the constellation
	 */
	unsigned get_n_symbols() const;

	/*
	 * \param idx is the index of the wanted symbol.
	 * \return the symbol of the constellation at the given index
	 */
	const S& operator[] (const size_t idx) const;
	const S& get_symbol (const size_t idx) const;

	/*
	 * \param idx is the index of the wanted symbol.
	 * \return the imaginary part of the constellation symbol at the given index
	 */
	R get_imag(const size_t idx) const;

	/*
	 * \param idx is the index of the wanted symbol.
	 * \return the real part of the constellation symbol at the given index
	 */
	R get_real(const size_t idx) const;

	/*
	 * \return false when all imaginary parts of the symbols are null
	 */
	bool is_complex() const;

	size_t size() const;

protected:
	/*
	 * \param n_bps is the number of bits per symbol
	 * \param name is the name of the constellation
	 */
	Constellation(const unsigned n_bps, const std::string& name);

	/*
	 * \param symbols are the symbols of the constellation that are copied
	 * \param name is the name of the constellation
	 */
	Constellation(const std::vector<S>& symbols, const std::string& name);

	/*
	 * \param symbols are the symbols that are semantically moved
	 * \param name is the name of the constellation
	 */
	Constellation(std::vector<S>&& symbols, const std::string& name);

	/*
	 * \param bits is the table of bits to map to the matching symbol.
	 *             Its length is of this->get_n_bits_per_symbol().
	 *             A bit is 0 or 1.
	 */
	virtual S bits_to_symbol(const uint8_t bits[]) const;

	/*
	 * \brief build the constellation itself by filling the constellation
	 *        with call to bits_to_symbol overloaded by the inheritance.
	 *        Must be called by the final constructor even when
	 *        bits_to_symbol is not overloaded, but after the constellation has
	 *        been filled.
	 */
	void build();

private:
	const unsigned n_bps;   // the number of bits per symbol
	const unsigned n_symbs; // the number of symbols
	const std::string name; // the name of the constellation
	std::vector<S> constellation; // the symbols of the constellation

	bool is_cplx; // false when all imaginary parts of the symbols are null
};

template <typename R>
bool has_complex_symbols(const Constellation<R>& cstl);

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Constellation/Constellation.hxx"
#endif

#endif // CONSTELLATION_HPP__