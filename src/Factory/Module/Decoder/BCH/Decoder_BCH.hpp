#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <cmath>

#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/Math/Galois.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_BCH : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const tools::Galois &GF) const;

		int t = 5; // correction power of th BCH
		int m;     // Gallois field order
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params, const tools::Galois &GF);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
