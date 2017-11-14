#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_RSC : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string      max           = "MAX";
		std::string      simd_strategy = "";
		std::string      standard      = "LTE";
		bool             buffered      = true;
		std::vector<int> poly          = {013, 015};

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Decoder_RSC::prefix);
		virtual ~parameters();
		Decoder_RSC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build(const std::vector<std::vector<int>> &trellis,
		                                            std::ostream                  &stream = std::cout,
		                                      const int                            n_ite  = 1) const;

	private:
		template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
		module::Decoder_SISO_SIHO<B,Q>* _build_seq(const std::vector<std::vector<int>> &trellis,
		                                                 std::ostream                  &stream = std::cout,
		                                           const int                            n_ite  = 1) const;

		template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max_i<Q> MAX>
		module::Decoder_SISO_SIHO<B,Q>* _build_simd(const std::vector<std::vector<int>> &trellis) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build(const parameters                    &params,
	                                             const std::vector<std::vector<int>> &trellis,
	                                                   std::ostream                  &stream = std::cout,
	                                             const int                            n_ite  = 1);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
