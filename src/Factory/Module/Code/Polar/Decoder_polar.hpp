#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include <string>
#include <mipp.h>
#include <cmath>

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_polar : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		std::string simd_strategy = "";
		std::string polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
		bool        full_adaptive = true;
		int         n_ite         = 1;
		int         L             = 8;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters& params, const std::vector<bool> &frozen_bits);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters& params, const std::vector<bool> &frozen_bits,
	                                        module::CRC<B> *crc = nullptr);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, const parameters& params, const bool full = true);

private:
	template <typename B = int, typename Q = float, class API_polar>
	static module::Decoder_SIHO<B,Q>* _build(const parameters& params, const std::vector<bool> &frozen_bits,
	                                         module::CRC<B> *crc = nullptr);

	template <typename B = int, typename Q = float, class API_polar>
	static module::Decoder_SIHO<B,Q>* _build_scl_fast(const parameters& params, const std::vector<bool> &frozen_bits,
	                                                  module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
