#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>

#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "../Factory_decoder_common.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_turbo : public Factory_decoder_common
{
	struct decoder_parameters_turbo : decoder_parameters
	{
		virtual ~decoder_parameters_turbo() {}

		// ------- decoder
		std::string max            = "MAX";;
		std::string simd_strategy  = "";
		std::string scaling_factor = "";
		bool        self_corrected = false;
		int         n_ite          = 6;

		bool        fnc            = false;
		int         fnc_q          = 10;
		int         fnc_ite_min    = 3;
		int         fnc_ite_max    = n_ite;
		int         fnc_ite_step   = 1;

		// ------- code
		std::vector<int> poly      = {013, 015};
//		int         tail_length    = 4 * 3;

		typename Factory_interleaver<int>::Interleaver_parameters itl;
	};

	static module::Decoder_turbo<B,R>* build(const std::string               type,
	                                         const std::string               implem,
	                                         const int                       K,
	                                         const int                       N,
	                                         const int                       n_ite,
	                                         const module::Interleaver<int> &itl,
	                                               module::SISO<R>          &siso_n,
	                                               module::SISO<R>          &siso_i,
	                                         const bool                      buffered = true);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_turbo &params, const int seed);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, Header::params_list& head_itl,
	                   const decoder_parameters_turbo& params, bool crc_activated);

};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
