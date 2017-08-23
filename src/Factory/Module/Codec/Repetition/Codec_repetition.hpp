#ifndef FACTORY_CODEC_REPETITION_HPP
#define FACTORY_CODEC_REPETITION_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"

#include "Module/Codec/Repetition/Codec_repetition.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_repetition : public Codec
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Codec::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q = float>
		module::Codec_repetition<B,Q>* build() const;

		Encoder_repetition::parameters enc;
		Decoder_repetition::parameters dec;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_repetition<B,Q>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, params_list& head_dec, const parameters& params,
	                        const bool full = true);
};
}
}

#endif /* FACTORY_CODEC_REPETITION_HPP */
