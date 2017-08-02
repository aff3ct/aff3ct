#ifndef FACTORY_DECODER_LDPC_HPP
#define FACTORY_DECODER_LDPC_HPP

#include <string>
#include <mipp.h>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_LDPC : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		std::string H_alist_path     = "";
		std::string simd_strategy    = "";
		float       norm_factor      = 1.f;
		float       offset           = 0.f;
		bool        enable_syndrome  = true;
		int         syndrome_depth   = 2;
		int         n_ite            = 10;
	};

	template <typename B = int, typename R = float>
	static module::Decoder_SISO_SIHO<B,R>* build(const parameters& params, const tools::Sparse_matrix &H,
	                                             const std::vector<unsigned> &info_bits_pos);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_DECODER_LDPC_HPP */
