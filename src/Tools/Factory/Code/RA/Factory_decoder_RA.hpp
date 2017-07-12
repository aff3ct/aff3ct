#ifndef FACTORY_DECODER_RA_HPP
#define FACTORY_DECODER_RA_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"

#include "../Factory_decoder.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_RA : public Factory_decoder
{
	struct parameters : Factory_decoder::parameters
	{
		virtual ~parameters() {}

		// ------- interleaver
		Factory_interleaver::parameters itl;

		// ------- decoder
		int n_ite = 10;
	};

	template <typename B = int, typename R = float>
	static module::Decoder<B,R>* build(const parameters &params, const module::Interleaver<int> &itl);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int K, const int N, int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_itl, const parameters& params);
};
}
}

#endif /* FACTORY_DECODER_RA_HPP */
