/*!
 * \file
 * \brief Class factory::Decoder_BCH.
 */
#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_BCH_name;
extern const std::string Decoder_BCH_prefix;
class Decoder_BCH : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int t = 5; // correction power of the BCH

	// deduced parameters
	int m = 0; // Gallois field order

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_BCH(const std::string &p = Decoder_BCH_prefix);
	virtual ~Decoder_BCH() = default;
	Decoder_BCH* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const tools::BCH_polynomial_generator<B> &GF,
	                                 module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
