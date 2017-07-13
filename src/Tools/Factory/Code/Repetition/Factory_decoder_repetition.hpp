#ifndef FACTORY_DECODER_REPETITION_HPP
#define FACTORY_DECODER_REPETITION_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"

#include "../Factory_decoder.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_repetition : public Factory_decoder
{
	template <typename B = int, typename R = float>
	static module::Decoder<B,R>* build(const Factory_decoder::parameters &params,
	                                   const bool buffered = true);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, Factory_decoder::parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_dec, const Factory_decoder::parameters& params);

};
}
}

#endif /* FACTORY_DECODER_REPETITION_HPP */
