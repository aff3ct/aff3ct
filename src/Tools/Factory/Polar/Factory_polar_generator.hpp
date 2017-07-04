#ifndef FACTORY_POLAR_GENERATOR_HPP
#define FACTORY_POLAR_GENERATOR_HPP

#include <string>
#include <mipp.h>

#include "Module/Decoder/Decoder.hpp"
#include "Module/CRC/CRC.hpp"

#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_polar_generator : public Factory_decoder_common
{
	struct parameters_polar_generator : decoder_parameters
	{
		virtual ~parameters_polar_generator() {}

		// ------- decoder
		std::string polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
		std::string gen_path;
		float       snr           = 0.f; // not noise var, used to set a fixed snr value for frozen bits construction (in polar codes)

//		float       sigma         = 0.f;
//		bool        full_adaptive = true;
//		int         n_ite         = 1;
//		int         L             = 8;

		// ------- code
		std::string bin_pb_path   = "../lib/polar_bounds/bin/polar_bounds";
		std::string awgn_fb_path  = "../conf/cde/awgn_polar_codes/TV";
		std::string fb_gen_method = "GA";
	};

	static module::Decoder<B,R>* build(const std::string      type,
	                                   const std::string      implem,
	                                   const int              K,
	                                   const int              N,
	                                   const mipp::vector<B> &frozen_bits,
	                                   const std::string      simd_strategy = "",
	                                   const bool             sys_encoding  = true,
	                                   const int              L             = 1,
	                                         module::CRC<B>  *crc           = nullptr,
	                                   const int              n_frames      = 1);

	static void get_frozen_bits(const std::string      implem,
	                            const int              N,
	                                  mipp::vector<B> &frozen_bits);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters_polar_generator &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, const parameters_polar_generator& params);

private:
	template <class API_polar>
	static module::Decoder<B,R>* _build(const std::string      type,
	                                    const std::string      implem,
	                                    const int              K,
	                                    const int              N,
	                                    const mipp::vector<B> &frozen_bits,
	                                    const int              L        = 1,
	                                          module::CRC<B>  *crc      = nullptr,
	                                    const int              n_frames = 1);
};
}
}

#endif /* FACTORY_POLAR_GENERATOR_HPP */
