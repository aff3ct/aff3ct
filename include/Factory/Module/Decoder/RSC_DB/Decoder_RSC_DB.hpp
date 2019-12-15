/*!
 * \file
 * \brief Class factory::Decoder_RSC_DB.
 */
#ifndef FACTORY_DECODER_RSC_DB_HPP
#define FACTORY_DECODER_RSC_DB_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Math/max.h"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RSC_DB_name;
extern const std::string Decoder_RSC_DB_prefix;
class Decoder_RSC_DB : public Decoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::string max      = "MAX";
	bool        buffered = true;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Decoder_RSC_DB(const std::string &p = Decoder_RSC_DB_prefix);
	virtual ~Decoder_RSC_DB() = default;
	Decoder_RSC_DB* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Decoder_RSC_DB_BCJR<B,Q>* build_siso(const std::vector<std::vector<int>> &trellis,
	                                             module::Encoder<B> *encoder = nullptr) const;

	template <typename B = int, typename Q = float>
	module::Decoder_SIHO<B,Q>* build(const std::vector<std::vector<int>> &trellis,
	                                 module::Encoder<B> *encoder = nullptr) const;

private:
	template <typename B = int, typename Q = float, tools::proto_max<Q> MAX>
	module::Decoder_RSC_DB_BCJR<B,Q>* _build_siso(const std::vector<std::vector<int>> &trellis,
	                                              module::Encoder<B> *encoder = nullptr) const;
};
}
}

#endif /* FACTORY_DECODER_RSC_DB_HPP */
