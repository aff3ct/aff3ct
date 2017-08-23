#ifndef FACTORY_CODEC_POLAR_HPP
#define FACTORY_CODEC_POLAR_HPP

#include <string>
#include <cmath>

#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"
#include "Factory/Module/Encoder/Polar/Encoder_polar.hpp"
#include "Factory/Module/Decoder/Polar/Decoder_polar.hpp"
#include "Factory/Module/Puncturer/Polar/Puncturer_polar.hpp"

#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Polar/Codec_polar.hpp"

#include "../Codec.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_polar : public Codec
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Codec::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q = float>
		module::Codec_polar<B,Q>* build(module::CRC<B> *crc = nullptr) const;

		Frozenbits_generator::parameters fbg;
		Encoder_polar       ::parameters enc;
		Decoder_polar       ::parameters dec;
		Puncturer_polar     ::parameters pct;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_polar<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, params_list& head_fbg, params_list& head_dec, params_list& head_pct,
	                        const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_CODEC_POLAR_HPP */
