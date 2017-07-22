#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_RSC : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		std::string      max           = "MAX";
		std::string      simd_strategy = "";
		std::string      standard      = "LTE";
		bool             buffered      = true;
		std::vector<int> poly          = {013, 015};
	};

	template <typename B = int, typename Q = float, typename QD = Q>
	static module::Decoder_SISO<B,Q>* build(const parameters                    &dec_par,
	                                        const std::vector<std::vector<int>> &trellis,
	                                              std::ostream                  &stream = std::cout,
	                                        const int                            n_ite  = 1);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p = prefix);
	static void header(params_list& head_dec, const parameters& params);

private:
	template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
	static module::Decoder_SISO<B,Q>* _build_seq(const parameters                    &dec_par,
	                                             const std::vector<std::vector<int>> &trellis,
	                                                   std::ostream                  &stream = std::cout,
	                                             const int                            n_ite  = 1);


	template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max_i<Q> MAX>
	static module::Decoder_SISO<B,Q>* _build_simd(const parameters                    &dec_par,
	                                              const std::vector<std::vector<int>> &trellis);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
