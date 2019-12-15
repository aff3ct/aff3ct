/*!
 * \file
 * \brief Class factory::Codec_RSC_DB.
 */
#ifndef FACTORY_CODEC_RSC_DB_HPP
#define FACTORY_CODEC_RSC_DB_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/RSC_DB/Codec_RSC_DB.hpp"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"
#include "Factory/Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_RSC_DB_name;
extern const std::string Codec_RSC_DB_prefix;
class Codec_RSC_DB : public Codec_SISO
{
public:
	explicit Codec_RSC_DB(const std::string &p = Codec_RSC_DB_prefix);
	virtual ~Codec_RSC_DB() = default;
	Codec_RSC_DB* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_RSC_DB<B,Q>* build(const module::CRC<B>* crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_RSC_DB_HPP */
