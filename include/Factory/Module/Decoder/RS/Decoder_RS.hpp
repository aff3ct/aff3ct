/*!
 * \file
 * \brief Class factory::Decoder_RS.
 */
#ifndef FACTORY_DECODER_RS_HPP
#define FACTORY_DECODER_RS_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RS_name;
extern const std::string Decoder_RS_prefix;
class Decoder_RS : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int t = 5; // correction power of the RS

	// deduced parameters
	int m = 0; // Gallois field order

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_RS(const std::string &p = Decoder_RS_prefix);
	virtual ~Decoder_RS() = default;
	Decoder_RS* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const tools::RS_polynomial_generator &GF,
	                                 module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_RS_HPP */
