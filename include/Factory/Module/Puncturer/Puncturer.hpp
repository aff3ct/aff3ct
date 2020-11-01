/*!
 * \file
 * \brief Class factory::Puncturer.
 */
#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Puncturer_name;
extern const std::string Puncturer_prefix;
class Puncturer : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         K    = 0;
	int         N    = 0;

	// optional parameters
	std::string type = "NO";
	int         N_cw = 0;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Puncturer(const std::string &p = Puncturer_prefix);
	virtual ~Puncturer() = default;
	virtual Puncturer* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B, typename Q>
	module::Puncturer<B,Q>* build() const;

protected:
	Puncturer(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_PONCTURER_HPP_ */
