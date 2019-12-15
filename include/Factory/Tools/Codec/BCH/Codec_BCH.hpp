/*!
 * \file
 * \brief Class factory::Codec_BCH.
 */
#ifndef FACTORY_CODEC_BCH_HPP
#define FACTORY_CODEC_BCH_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/BCH/Codec_BCH.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_BCH_name;
extern const std::string Codec_BCH_prefix;
class Codec_BCH : public Codec_SIHO
{
public:
	explicit Codec_BCH(const std::string &p = Codec_BCH_prefix);
	virtual ~Codec_BCH() = default;
	Codec_BCH* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_BCH<B,Q>* build(const module::CRC<B> *crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_BCH_HPP */
