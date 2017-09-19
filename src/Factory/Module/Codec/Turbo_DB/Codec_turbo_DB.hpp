#ifndef FACTORY_CODEC_TURBO_DB_HPP
#define FACTORY_CODEC_TURBO_DB_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"

#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Turbo_DB/Codec_turbo_DB.hpp"

#include "../Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_turbo_DB : public Codec_SIHO
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Codec_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_turbo_DB  ::parameters *enc;
		Decoder_turbo_DB  ::parameters *dec;
		Puncturer_turbo_DB::parameters *pct;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Codec_turbo_DB::prefix);
		virtual ~parameters();
		Codec_turbo_DB::parameters* clone() const;
		void enable_puncturer();

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
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
