/*!
 * \file
 * \brief Class factory::Encoder_RS.
 */
#ifndef FACTORY_ENCODER_RS_HPP
#define FACTORY_ENCODER_RS_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Encoder/RS/Encoder_RS.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RS_name;
extern const std::string Encoder_RS_prefix;
class Encoder_RS : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_RS(const std::string &p = Encoder_RS_prefix);
	virtual ~Encoder_RS() = default;
	Encoder_RS* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_RS<B>* build(const tools::RS_polynomial_generator &GF) const;
};
}
}

#endif /* FACTORY_ENCODER_RS_HPP */
