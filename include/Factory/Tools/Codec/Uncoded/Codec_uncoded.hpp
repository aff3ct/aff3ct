/*!
 * \file
 * \brief Class factory::Codec_uncoded.
 */
#ifndef FACTORY_CODEC_UNCODED_HPP
#define FACTORY_CODEC_UNCODED_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/Uncoded/Codec_uncoded.hpp"
#include "Factory/Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_uncoded_name;
extern const std::string Codec_uncoded_prefix;
class Codec_uncoded : public Codec_SISO
{
public:
	explicit Codec_uncoded(const std::string &p = Codec_uncoded_prefix);
	virtual ~Codec_uncoded() = default;
	Codec_uncoded* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_uncoded<B,Q>* build(const module::CRC<B>* crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_UNCODED_HPP */
