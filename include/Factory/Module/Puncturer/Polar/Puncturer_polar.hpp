/*!
 * \file
 * \brief Class factory::Puncturer_polar.
 */
#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Module/Puncturer/Polar/Puncturer_polar_shortlast.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Puncturer_polar_name;
extern const std::string Puncturer_polar_prefix;
class Puncturer_polar : public Puncturer
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Puncturer_polar(const std::string &p = Puncturer_polar_prefix);
	virtual ~Puncturer_polar() = default;
	Puncturer_polar* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Puncturer_polar_shortlast<B,Q>* build(const tools::Frozenbits_generator &fb_generator) const;
};
}
}

#endif /* FACTORY_PUNCTURER_POLAR_HPP */
