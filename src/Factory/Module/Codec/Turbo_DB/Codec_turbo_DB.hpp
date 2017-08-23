#ifndef FACTORY_CODEC_TURBO_DB_HPP
#define FACTORY_CODEC_TURBO_DB_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Factory/Module/Puncturer/Turbo_DB/Puncturer_turbo_DB.hpp"

#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Turbo_DB/Codec_turbo_DB.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_turbo_DB : public Codec
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Codec::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q>
		module::Codec_turbo_DB<B,Q>* build(module::CRC<B> *crc = nullptr) const;

		Encoder_turbo_DB  ::parameters enc;
		Decoder_turbo_DB  ::parameters dec;
		Puncturer_turbo_DB::parameters pct;
	};

	template <typename B = int, typename Q>
	static module::Codec_turbo_DB<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, params_list& head_dec, params_list& head_pct, params_list& head_itl,
	                        const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_CODEC_TURBO_DB_HPP */
