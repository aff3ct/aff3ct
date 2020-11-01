/*!
 * \file
 * \brief Class factory::Coset.
 */
#ifndef FACTORY_COSET_HPP
#define FACTORY_COSET_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Coset/Coset.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Coset_name;
extern const std::string Coset_prefix;
class Coset : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         size     = 0;

	// optional parameters
	std::string type     = "STD";

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Coset(const std::string &p = Coset_prefix);
	virtual ~Coset() = default;
	Coset* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B1 = int, typename B2 = B1>
	module::Coset<B1,B2>* build_bit() const;

	template <typename B = int, typename R = float>
	module::Coset<B,R>* build_real() const;
};
}
}

#endif /* FACTORY_COSET_HPP */
