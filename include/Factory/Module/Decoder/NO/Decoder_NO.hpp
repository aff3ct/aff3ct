/*!
 * \file
 * \brief Class factory::Decoder_NO.
 */
#ifndef FACTORY_DECODER_NO_HPP
#define FACTORY_DECODER_NO_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_NO_name;
extern const std::string Decoder_NO_prefix;
class Decoder_NO : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_NO(const std::string &p = Decoder_NO_prefix);
	virtual ~Decoder_NO() = default;
	Decoder_NO* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SISO<B,Q>* build_siso(module::Encoder<B> *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_NO_HPP */
