/*!
 * \file
 * \brief Class factory::Codec_RS.
 */
#ifndef FACTORY_CODEC_RS_HPP
#define FACTORY_CODEC_RS_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/RS/Codec_RS.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_RS_name;
extern const std::string Codec_RS_prefix;
class Codec_RS : public Codec_SIHO
{
public:
	explicit Codec_RS(const std::string &p = Codec_RS_prefix);
	virtual ~Codec_RS() = default;
	Codec_RS* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_RS<B,Q>* build(const module::CRC<B> *crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_RS_HPP */
