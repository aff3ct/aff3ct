#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <cmath>

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
	struct decoder_parameters_BCH : decoder_parameters
	{
		virtual ~decoder_parameters_BCH() {}

		// ------- code
		int t = 5; // correction power of th BCH
		int m;     // Gallois field order
	};

	static module::Decoder<B,Q>* build(const decoder_parameters_BCH &params, const Galois &GF);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_BCH &params, int K, int N, int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, const decoder_parameters_BCH& params);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
