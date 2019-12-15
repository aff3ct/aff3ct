/*!
 * \file
 * \brief Class factory::Decoder_RSC.
 */
#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Math/max.h"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RSC_name;
extern const std::string Decoder_RSC_prefix;
class Decoder_RSC : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::string      max           = "MAX";
	std::string      simd_strategy = "";
	std::string      standard      = "LTE";
	bool             buffered      = true;
	std::vector<int> poly          = {013, 015};

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_RSC(const std::string &p = Decoder_RSC_prefix);
	virtual ~Decoder_RSC() = default;
	Decoder_RSC* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const std::vector<std::vector<int>> &trellis,
	                                       std::ostream                  &stream  = std::cout,
	                                 const int                            n_ite   = 1,
	                                       module::Encoder<B>            *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SISO<B,Q>* build_siso(const std::vector<std::vector<int>> &trellis,
	                                            std::ostream                  &stream  = std::cout,
	                                      const int                            n_ite   = 1,
	                                            module::Encoder<B>            *encoder = nullptr) const;

private:
	template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max<Q> MAX1, tools::proto_max<QD> MAX2>
	module::Decoder_SISO<B,Q>* _build_siso_seq(const std::vector<std::vector<int>> &trellis,
	                                                 std::ostream                  &stream  = std::cout,
	                                           const int                            n_ite   = 1,
	                                                 module::Encoder<B>            *encoder = nullptr) const;

	template <typename B = int, typename Q = float, typename QD = Q, tools::proto_max_i<Q> MAX>
	module::Decoder_SISO<B,Q>* _build_siso_simd(const std::vector<std::vector<int>> &trellis,
	                                                  module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_RSC_HPP */
