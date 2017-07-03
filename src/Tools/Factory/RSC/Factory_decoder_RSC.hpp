#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory_decoder_common.hpp"
#include "Factory_encoder_RSC.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float, typename QD = Q>
struct Factory_decoder_RSC : public Factory_decoder_common
{
	struct decoder_parameters_RSC : decoder_parameters
	{
		virtual ~decoder_parameters_RSC() {}

		// ------- decoder
		std::string max              = "MAXS";
		std::string simd_strategy    = "";
	};

	static module::Decoder_SISO<B,Q>* build(const decoder_parameters_RSC                                  &dec_par,
	                                        const typename Factory_encoder_RSC<B>::encoder_parameters_RSC &enc_par,
	                                        const std::vector<std::vector<int>>                           &trellis,
	                                              std::ostream                                            &stream = std::cout,
	                                        const int                                                      n_ite  = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_RSC &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, const decoder_parameters_RSC& params);

private:
	template <proto_max<Q> MAX1, proto_max<QD> MAX2>
	static module::Decoder_SISO<B,Q>* _build_seq(const decoder_parameters_RSC                                  &dec_par,
	                                             const typename Factory_encoder_RSC<B>::encoder_parameters_RSC &enc_par,
	                                             const std::vector<std::vector<int>>                           &trellis,
	                                                   std::ostream                                            &stream = std::cout,
	                                             const int                                                      n_ite  = 1);


	template <proto_max_i<Q> MAX>
	static module::Decoder_SISO<B,Q>* _build_simd(const decoder_parameters_RSC                                  &dec_par,
	                                              const typename Factory_encoder_RSC<B>::encoder_parameters_RSC &enc_par,
	                                              const std::vector<std::vector<int>>                           &trellis);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
