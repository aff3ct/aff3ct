#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <string>
#include <mipp.h>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory_decoder.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_LDPC : public Factory_decoder
{
	struct parameters : Factory_decoder::parameters
	{
		virtual ~parameters() {}

		// ------- decoder
		std::string simd_strategy    = "";
		float       norm_factor      = 1.f;
		float       offset           = 0.f;
		bool        enable_syndrome  = true;
		int         syndrome_depth   = 2;
		int         n_ite            = 10;

		// ------- code
		std::string alist_path = "";
	};

	template <typename B = int, typename R = float>
	static module::Decoder_SISO<B,R>* build(const parameters& params, const Sparse_matrix &H,
	                                        const std::vector<unsigned> &info_bits_pos);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, const parameters& params);
};
}
}

#endif /* FACTORY_DECODER_LDPC_HPP */
