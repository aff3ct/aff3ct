#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>
#include <type_traits>

#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "../Factory_decoder_common.hpp"
#include "Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/Turbo/Factory_flip_and_check.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_decoder_turbo : public Factory_decoder_common
{
	struct decoder_parameters_turbo : decoder_parameters
	{
		virtual ~decoder_parameters_turbo() {}

		// ------- decoder
		std::string max            = "MAX";;
		std::string simd_strategy  = "";
		std::string turbo_implem   = "";
		bool        self_corrected = false;
		int         n_ite          = 6;

		typename Factory_scaling_factor<B,Q>::scaling_factor_parameters scaling_factor;
		typename Factory_flip_and_check<B,Q>::flip_and_check_parameters flip_and_check;
	};

	static module::Decoder_turbo<B,Q>* build(const decoder_parameters_turbo &params,
	                                         const module::Interleaver<int> &itl,
	                                               module::SISO<Q>          &siso_n,
	                                               module::SISO<Q>          &siso_i,
	                                         const bool                      buffered = true);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_turbo &params,
	                       const int K, const int N, const int n_frames = 1,
	                       const bool activate_simd = true, const bool activate_json = false);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, const decoder_parameters_turbo& params, bool crc_activated);

};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
