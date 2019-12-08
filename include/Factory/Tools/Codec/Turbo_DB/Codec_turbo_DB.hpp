/*!
 * \file
 * \brief Class factory::Codec_turbo_DB.
 */
#ifndef FACTORY_CODEC_TURBO_DB_HPP
#define FACTORY_CODEC_TURBO_DB_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"
#include "Tools/Codec/Turbo_DB/Codec_turbo_DB.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_turbo_DB_name;
extern const std::string Codec_turbo_DB_prefix;
class Codec_turbo_DB : public Codec_SIHO
{
public:
	explicit Codec_turbo_DB(const std::string &p = Codec_turbo_DB_prefix);
	virtual ~Codec_turbo_DB() = default;
	Codec_turbo_DB* clone() const;
	void enable_puncturer();

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_turbo_DB<B,Q>* build(const module::CRC<B>* crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_TURBO_DB_HPP */
