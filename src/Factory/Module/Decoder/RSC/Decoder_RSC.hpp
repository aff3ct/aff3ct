#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Tools/Math/max.h"

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RSC_name;
extern const std::string Decoder_RSC_prefix;
struct Decoder_RSC : public Decoder
{
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
		explicit parameters(const std::string &p = Decoder_RSC_prefix);
		virtual ~parameters();
		Decoder_RSC::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const std::vector<std::vector<int>> &trellis,
		                                       std::ostream                  &stream  = std::cout,
		                                 const int                            n_ite   = 1,
		                                       module::Encoder_RSC_sys<B>    *encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const std::vector<std::vector<int>> &trellis,
		                                                 std::ostream                  &stream  = std::cout,
		                                           const int                            n_ite   = 1,
		                                                 module::Encoder_RSC_sys<B>    *encoder = nullptr) const;

	private:
		template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
		module::Decoder_SISO_SIHO<B,Q>* _build_siso_seq(const std::vector<std::vector<int>> &trellis,
		                                                      std::ostream                  &stream  = std::cout,
		                                                const int                            n_ite   = 1,
		                                                      module::Encoder_RSC_sys<B>    *encoder = nullptr) const;

		template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max_i<Q> MAX>
		module::Decoder_SISO_SIHO<B,Q>* _build_siso_simd(const std::vector<std::vector<int>> &trellis,
		                                                       module::Encoder_RSC_sys<B>    *encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters                    &params,
	                                        const std::vector<std::vector<int>> &trellis,
	                                              std::ostream                  &stream  = std::cout,
	                                        const int                            n_ite   = 1,
	                                              module::Encoder_RSC_sys<B>    *encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters                    &params,
	                                                  const std::vector<std::vector<int>> &trellis,
	                                                        std::ostream                  &stream  = std::cout,
	                                                  const int                            n_ite   = 1,
	                                                        module::Encoder_RSC_sys<B>    *encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
