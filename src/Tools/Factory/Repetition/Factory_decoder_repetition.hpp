#ifndef FACTORY_DECODER_REPETITION_HPP
#define FACTORY_DECODER_REPETITION_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"

#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_repetition : public Factory_decoder_common
{
	static module::Decoder<B,R>* build(const std::string type,
	                                   const std::string implem,
	                                   const int         K,
	                                   const int         N,
	                                   const bool        buffered = true,
	                                   const int         n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, typename Factory_decoder_common::decoder_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, const typename Factory_decoder_common::decoder_parameters& params);

};
}
}

#endif /* FACTORY_DECODER_REPETITION_HPP */
