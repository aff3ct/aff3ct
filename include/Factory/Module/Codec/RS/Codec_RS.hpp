/*!
 * \file
 * \brief Class factory::Codec_RS.
 */
#ifndef FACTORY_CODEC_RS_HPP
#define FACTORY_CODEC_RS_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Module/CRC/CRC.hpp"
#include "Module/Codec/RS/Codec_RS.hpp"
#include "Factory/Module/Codec/Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_RS_name;
extern const std::string Codec_RS_prefix;
class Codec_RS : public Codec_SIHO_HIHO
{
public:
	explicit Codec_RS(const std::string &p = Codec_RS_prefix);
	virtual ~Codec_RS() = default;
	Codec_RS* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Codec_RS<B,Q>* build(module::CRC<B> *crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_RS_HPP */
