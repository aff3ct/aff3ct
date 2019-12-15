/*!
 * \file
 * \brief Class factory::Codec_turbo.
 */
#ifndef FACTORY_CODEC_TURBO_HPP
#define FACTORY_CODEC_TURBO_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/Turbo/Codec_turbo.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_turbo_name;
extern const std::string Codec_turbo_prefix;
class Codec_turbo : public Codec_SIHO
{
public:
	explicit Codec_turbo(const std::string &p = Codec_turbo_prefix);
	virtual ~Codec_turbo() = default;
	Codec_turbo* clone() const;
	void enable_puncturer();

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_turbo<B,Q>* build(const module::CRC<B> *crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_TURBO_HPP */
