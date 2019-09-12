#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Math/max.h"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

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
		virtual ~parameters() = default;
		Decoder_RSC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const std::vector<std::vector<int>>       &trellis,
		                                       std::ostream                        &stream  = std::cout,
		                                 const int                                  n_ite   = 1,
		                                 const std::unique_ptr<module::Encoder<B>> &encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const std::vector<std::vector<int>>       &trellis,
		                                                 std::ostream                        &stream  = std::cout,
		                                           const int                                  n_ite   = 1,
		                                           const std::unique_ptr<module::Encoder<B>> &encoder = nullptr) const;

	private:
		template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
		module::Decoder_SISO_SIHO<B,Q>* _build_siso_seq(const std::vector<std::vector<int>>       &trellis,
		                                                      std::ostream                        &stream  = std::cout,
		                                                const int                                  n_ite   = 1,
		                                                const std::unique_ptr<module::Encoder<B>> &encoder = nullptr) const;

		template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max_i<Q> MAX>
		module::Decoder_SISO_SIHO<B,Q>* _build_siso_simd(const std::vector<std::vector<int>> &trellis,
		                                                 const std::unique_ptr<module::Encoder<B>> &encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters                          &params,
	                                        const std::vector<std::vector<int>>       &trellis,
	                                              std::ostream                        &stream  = std::cout,
	                                        const int                                  n_ite   = 1,
	                                        const std::unique_ptr<module::Encoder<B>> &encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters                          &params,
	                                                  const std::vector<std::vector<int>>       &trellis,
	                                                        std::ostream                        &stream  = std::cout,
	                                                  const int                                  n_ite   = 1,
	                                                  const std::unique_ptr<module::Encoder<B>> &encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
