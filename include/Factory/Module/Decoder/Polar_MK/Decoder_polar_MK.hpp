/*!
 * \file
 * \brief Class factory::Decoder_polar_MK.
 */
#ifndef FACTORY_DECODER_POLAR_MK_HPP
#define FACTORY_DECODER_POLAR_MK_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_polar_MK_name;
extern const std::string Decoder_polar_MK_prefix;
class Decoder_polar_MK : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	int         L         = 8;
	std::string node_type = "MS";

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_polar_MK(const std::string &p = Decoder_polar_MK_prefix);
	virtual ~Decoder_polar_MK() = default;
	Decoder_polar_MK* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const tools::Polar_code &code, const std::vector<bool> &frozen_bits,
	                                 const module::CRC<B> *crc = nullptr,
	                                       module::Encoder<B> *encoder = nullptr) const;

protected:
	Decoder_polar_MK(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_DECODER_POLAR_MK_HPP */
