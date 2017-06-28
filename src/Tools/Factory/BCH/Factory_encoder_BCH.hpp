#ifndef FACTORY_ENCODER_BCH_HPP
#define FACTORY_ENCODER_BCH_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"
#include "Tools/Math/Galois.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_BCH : public Factory_encoder_common<B>
{
	static module::Encoder<B>* build(const std::string  type,
	                                 const int          K,
	                                 const int          N,
	                                 const Galois      &GF,
	                                 const int          n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, typename Factory_encoder_common<B>::encoder_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_ENCODER_BCH_HPP */
