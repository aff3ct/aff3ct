#ifndef FACTORY_DECODER_RA_HPP
#define FACTORY_DECODER_RA_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"

#include "../../Interleaver.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_RA : public Decoder
{
	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		int n_ite = 10;

		Interleaver::parameters itl;
	};

	template <typename B = int, typename R = float>
	static module::Decoder<B,R>* build(const parameters &params, const module::Interleaver<int> &itl);

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_dec, params_list& head_itl, const parameters& params);
};
}
}

#endif /* FACTORY_DECODER_RA_HPP */
