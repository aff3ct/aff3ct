#ifndef FACTORY_CODEC_TURBO_DB_HPP
#define FACTORY_CODEC_TURBO_DB_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"
#include "Module/Codec/Turbo_DB/Codec_turbo_DB.hpp"
#include "Factory/Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_turbo_DB_name;
extern const std::string Codec_turbo_DB_prefix;
struct Codec_turbo_DB : public Codec_SIHO
{
	class parameters : public Codec_SIHO::parameters
	{
	public:
		explicit parameters(const std::string &p = Codec_turbo_DB_prefix);
		virtual ~parameters() = default;
		Codec_turbo_DB::parameters* clone() const;
		void enable_puncturer();

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_turbo_DB<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q>
	static module::Codec_turbo_DB<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_TURBO_DB_HPP */
