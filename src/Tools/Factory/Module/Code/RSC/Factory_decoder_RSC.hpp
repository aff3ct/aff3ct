#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory_decoder.hpp"
#include "Factory_encoder_RSC.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_RSC : public Factory_decoder
{
	struct parameters : Factory_decoder::parameters
	{
		virtual ~parameters() {}

		std::string      max           = "MAXS";
		std::string      simd_strategy = "";
		std::string      standard      = "LTE";
		bool             buffered      = true;
		std::vector<int> poly          = {013, 015};
	};

	template <typename B = int, typename Q = float, typename QD = Q>
	static module::Decoder_SISO<B,Q>* build(const parameters                      &dec_par,
	                                        const Factory_encoder_RSC::parameters &enc_par,
	                                        const std::vector<std::vector<int>>   &trellis,
	                                              std::ostream                    &stream = std::cout,
	                                        const int                              n_ite  = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_dec, const parameters& params);

private:
	template <typename B = int, typename Q = float, typename QD = Q, proto_max<Q> MAX1, proto_max<QD> MAX2>
	static module::Decoder_SISO<B,Q>* _build_seq(const parameters                      &dec_par,
	                                             const Factory_encoder_RSC::parameters &enc_par,
	                                             const std::vector<std::vector<int>>   &trellis,
	                                                   std::ostream                    &stream = std::cout,
	                                             const int                              n_ite  = 1);


	template <typename B = int, typename Q = float, typename QD = Q, proto_max_i<Q> MAX>
	static module::Decoder_SISO<B,Q>* _build_simd(const parameters                      &dec_par,
	                                              const Factory_encoder_RSC::parameters &enc_par,
	                                              const std::vector<std::vector<int>>   &trellis);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
