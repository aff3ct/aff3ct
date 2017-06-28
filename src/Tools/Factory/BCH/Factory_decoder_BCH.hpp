#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Tools/Math/Galois.hpp"

#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_decoder_BCH : public Factory_decoder_common
{
	static module::Decoder<B,Q>* build(const std::string  type,
	                                   const std::string  implem,
	                                   const int          K,
	                                   const int          N,
	                                   const int          t,
	                                   const Galois      &GF,
	                                   const int          n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
