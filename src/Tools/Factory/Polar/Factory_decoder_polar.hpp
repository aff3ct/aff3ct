#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include <string>
#include <mipp.h>
#include <cmath>

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Decoder.hpp"

#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_decoder_polar : public Factory_decoder_common
{
	struct decoder_parameters_polar : decoder_parameters
	{
		virtual ~decoder_parameters_polar() {}

		// ------- decoder
		std::string simd_strategy = "";
		std::string polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
		bool        full_adaptive = true;
		int         n_ite         = 1;
		int         L             = 8;

		// ------- code
		std::string bin_pb_path   = "../lib/polar_bounds/bin/polar_bounds";
		std::string awgn_fb_path  = "../conf/cde/awgn_polar_codes/TV";
		std::string fb_gen_method = "GA";
		float       sigma         = 0.f; // not noise var, used to set a fixed snr value for frozen bits construction (in polar codes)
		int         N_pct;               // size of the frame at the ouput of the poncturer
	};

	static module::Decoder_SISO<B,Q>* build_siso(const decoder_parameters_polar& params, const mipp::vector<B> &frozen_bits, const bool sys_encoding = true);

	static module::Decoder<B,Q>* build(const decoder_parameters_polar& params, const mipp::vector<B> &frozen_bits, const bool sys_encoding = true, module::CRC<B> *crc = nullptr);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_polar &params, int K, int N, int n_frames);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, const decoder_parameters_polar& params);

private:
	template <class API_polar>
	static module::Decoder<B,Q>* _build(const decoder_parameters_polar& params, const mipp::vector<B> &frozen_bits, const bool sys_encoding = true, module::CRC<B> *crc = nullptr);

	template <class API_polar>
	static module::Decoder<B,Q>* _build_scl_fast(const decoder_parameters_polar& params, const mipp::vector<B> &frozen_bits, const bool sys_encoding = true, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
