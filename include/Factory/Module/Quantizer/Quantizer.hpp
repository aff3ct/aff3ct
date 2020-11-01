/*!
 * \file
 * \brief Class factory::Quantizer.
 */
#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Quantizer_name;
extern const std::string Quantizer_prefix;
class Quantizer : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         size       = 0;

	// optional parameters
	std::string type       = "POW2";
	std::string implem     = "STD";
	float       range      = 0.f;
	int         n_bits     = 8;
	int         n_decimals = 3;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Quantizer(const std::string &p = Quantizer_prefix);
	virtual ~Quantizer() = default;
	Quantizer* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename R = float, typename Q = R>
	module::Quantizer<R,Q>* build() const;
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
