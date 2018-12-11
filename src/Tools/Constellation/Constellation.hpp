#ifndef CONSTELLATION_HPP__
#define CONSTELLATION_HPP__

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
class Constellation
{
public:
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
	 * \brief build the constellation itself.
	 *        must be called by the final constructor
	 */
	void build();

protected:
	/*
	 * \param n_bps is the number of bits per symbol
	 * \param name is the name of the constellation
	 */
	Constellation(const unsigned n_bps, const std::string& name);

private:
	const unsigned n_bps;   // the number of bits per symbol
	const unsigned n_symbs; // the number of symbols
	const std::string name; // the name of the constellation
};

/*
 * \brief describe a constellation
 * \param Symbol_type is the symbol type as a real or a complex
 */
template <class Symbol_type>
class Constellation_S : public Constellation
{
public:
	using S = Symbol_type;

	virtual ~Constellation_S() = default;

	/*
	 * \param idx is the index of the wanted symbol.
	 * \return the symbol of the constellation at the given index
	 */
	const S operator[] (const size_t idx) const;
	const S get_symbol (const size_t idx) const;

	/*
	 * \brief build the constellation itself.
	 *        must be called by the final constructor
	 */
	void build();

protected:
	/*
	 * \param n_bps is the number of bits per symbol
	 * \param name is the name of the constellation
	 */
	Constellation_S(const unsigned n_bps, const std::string& name);

	/*
	 * \param symbols are the symbols of the constellation that are copied
	 * \param name is the name of the constellation
	 */
	Constellation_S(const std::vector<S>& symbols, const std::string& name);

	/*
	 * \param symbols are the symbols that are semantically moved
	 * \param name is the name of the constellation
	 */
	Constellation_S(std::vector<S>&& symbols, const std::string& name);

	/*
	 * \param bits is the table of bits to map to the matching symbol.
	 *             Its length is of this->get_n_bits_per_symbol().
	 *             A bit is 0 or 1.
	 */
	virtual S bits_to_symbol(const uint8_t bits[]) const;

protected:
	std::vector<S> constellation; // the symbols of the constellation
};
}
}

#include "Constellation.hxx"

#endif // CONSTELLATION_HPP__