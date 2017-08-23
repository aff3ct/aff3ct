#ifndef FACTORY_CODEC_BCH_HPP
#define FACTORY_CODEC_BCH_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Module/Codec/BCH/Codec_BCH.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_BCH : public Codec
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Codec::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q = float>
		module::Codec_BCH<B,Q>* build() const;

		Encoder_BCH::parameters enc;
		Decoder_BCH::parameters dec;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_BCH<B,Q>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, params_list& head_dec, const parameters& params,
	                        const bool full = true);
};
}
}

#endif /* FACTORY_CODEC_BCH_HPP */
