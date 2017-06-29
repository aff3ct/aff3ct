#ifndef FACTORY_DECODER_RA_HPP
#define FACTORY_DECODER_RA_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"

#include "../Factory_decoder_common.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_RA : public Factory_decoder_common
{
	struct decoder_parameters_RA : decoder_parameters
	{
		virtual ~decoder_parameters_RA() {}
		// ------- code
		typename Factory_interleaver<int>::Interleaver_parameters itl;

		// ------- decoder
		int n_ite = 10;
	};

	static module::Decoder<B,R>* build(const std::string               type,
	                                   const std::string               implem,
	                                   const int                       K,
	                                   const int                       N,
	                                   const module::Interleaver<int> &itl,
	                                   const int                       n_ite,
	                                   const int                       n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_RA &params, const int seed);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_itl, const decoder_parameters_RA& params);
};
}
}

#endif /* FACTORY_DECODER_RA_HPP */
