#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include <string>
#include <mipp.h>
#include <cmath>

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Decoder.hpp"

#include "../Factory_decoder.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_polar : public Factory_decoder
{
	struct parameters : Factory_decoder::parameters
	{
		virtual ~parameters() {}

		std::string simd_strategy = "";
		std::string polar_nodes   = "{R0,R0L,R1,REP,REPL,SPC}";
		bool        full_adaptive = true;
		int         n_ite         = 1;
		int         L             = 8;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO<B,Q>* build_siso(const parameters& params, const mipp::vector<B> &frozen_bits);

	template <typename B = int, typename Q = float>
	static module::Decoder<B,Q>* build(const parameters& params, const mipp::vector<B> &frozen_bits,
	                                   module::CRC<B> *crc = nullptr);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_dec, const parameters& params);

private:
	template <typename B = int, typename Q = float, class API_polar>
	static module::Decoder<B,Q>* _build(const parameters& params, const mipp::vector<B> &frozen_bits,
	                                    module::CRC<B> *crc = nullptr);

	template <typename B = int, typename Q = float, class API_polar>
	static module::Decoder<B,Q>* _build_scl_fast(const parameters& params, const mipp::vector<B> &frozen_bits,
	                                             module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_DECODER_POLAR_HPP */
