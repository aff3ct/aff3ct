#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <cmath>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Tools/Math/Galois.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_BCH : public Decoder
{
	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		int t = 5; // correction power of th BCH
		int m;     // Gallois field order
	};

	template <typename B = int, typename Q = float>
	static module::Decoder<B,Q>* build(const parameters &params, const tools::Galois &GF);

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_dec, const parameters& params);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
