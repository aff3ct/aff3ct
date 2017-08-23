#ifndef FACTORY_CODEC_RA_HPP
#define FACTORY_CODEC_RA_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/RA/Encoder_RA.hpp"
#include "Factory/Module/Decoder/RA/Decoder_RA.hpp"

#include "Module/Codec/RA/Codec_RA.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_RA : public Codec
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Codec::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q = float>
		module::Codec_RA<B,Q>* build() const;

		Encoder_RA::parameters enc;
		Decoder_RA::parameters dec;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_RA<B,Q>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, params_list& head_dec, params_list& head_itl,
	                        const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_CODEC_RA_HPP */
