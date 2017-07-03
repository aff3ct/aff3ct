#ifndef FACTORY_DECODER_NO_HPP
#define FACTORY_DECODER_NO_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_decoder_NO : public Factory_decoder_common
{
	static module::Decoder<B,Q>* build(const std::string  type,
	                                   const std::string  implem,
	                                   const int          K,
	                                   const int          n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, const decoder_parameters& params);
};
}
}

#endif /* FACTORY_DECODER_NO_HPP */
