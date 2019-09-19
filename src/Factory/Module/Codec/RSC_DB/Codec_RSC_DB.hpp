#ifndef FACTORY_CODEC_RSC_DB_HPP
#define FACTORY_CODEC_RSC_DB_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Module/Codec/RSC_DB/Codec_RSC_DB.hpp"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Decoder/RSC_DB/Decoder_RSC_DB.hpp"
#include "Factory/Module/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_RSC_DB_name;
extern const std::string Codec_RSC_DB_prefix;
struct Codec_RSC_DB : public Codec_SISO_SIHO
{
	struct parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		explicit parameters(const std::string &p = Codec_RSC_DB_prefix);
		virtual ~parameters() = default;
		Codec_RSC_DB::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_RSC_DB<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_RSC_DB<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_RSC_DB_HPP */
