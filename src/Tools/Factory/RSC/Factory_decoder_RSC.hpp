#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float, typename RD = R>
struct Factory_decoder_RSC : public Factory_decoder_common
{
	struct decoder_parameters_RSC : decoder_parameters
	{
		virtual ~decoder_parameters_RSC() {}

		// ------- decoder
		std::string max           = "MAX";;
		std::string simd_strategy = "";

		// ------- code
		std::vector<int> poly = {013, 015};
	};

	static module::Decoder_SISO<B,R>* build(const std::string                    type,
	                                        const std::string                    implem,
	                                        const int                            K,
	                                        const std::vector<std::vector<int>> &trellis,
	                                        const std::string                    max_type      = "MAX",
	                                        const std::string                    simd_strategy = "",
	                                        const bool                           buffered      = true,
	                                              std::ostream                  &stream        = std::cout,
	                                        const int                            n_ite         = 1,
	                                        const int                            n_frames      = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, decoder_parameters_RSC &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, const decoder_parameters_RSC& params);

private:
	template <proto_max<R> MAX1, proto_max<RD> MAX2>
	static module::Decoder_SISO<B,R>* _build_seq(const std::string                    type,
	                                             const std::string                    implem,
	                                             const int                            K,
	                                             const std::vector<std::vector<int>> &trellis,
	                                             const bool                           buffered = true,
	                                                   std::ostream                  &stream   = std::cout,
	                                             const int                            n_ite    = 1,
	                                             const int                            n_frames = 1);

	template <proto_max_i<R> MAX>
	static module::Decoder_SISO<B,R>* _build_simd(const std::string                    type,
	                                              const std::string                    implem,
	                                              const int                            K,
	                                              const std::vector<std::vector<int>> &trellis,
	                                              const std::string                    simd_strategy = "INTRA",
	                                              const bool                           buffered      = true,
	                                              const int                            n_frames      = 1);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
