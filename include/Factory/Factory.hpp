/*!
 * \file
 * \brief Class factory::Factory.
 */
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Interface/Interface_clone.hpp"
#include "Tools/Factory/Header.hpp"

namespace aff3ct
{
namespace factory
{

extern const std::string Factory_name;
extern const std::string Factory_short_name;
extern const std::string Factory_prefix;
class Factory : public tools::Interface_clone
{
public:
	// constructor/destructor
	Factory(const std::string &name       = Factory_name,
	        const std::string &short_name = Factory_short_name,
	        const std::string &prefix     = Factory_prefix);
	virtual ~Factory() = default;

	virtual Factory*                 clone          () const = 0;
	                    std::string  get_name       () const;
	                    std::string  get_short_name () const;
	                    std::string  get_prefix     () const;
	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	virtual void get_description(cli::Argument_map_info &args) const = 0;
	virtual void store          (const cli::Argument_map_value &vals                                      ) = 0;
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const = 0;

	static cli::Argument_map_info get_description(const std::vector<Factory*> &factories);
	static void get_description(const std::vector<Factory*> &factories, cli::Argument_map_info &args);

	static void                    store        (std::vector<Factory*> &factories, const cli::Argument_map_value &vals);
	static cli::Argument_map_group create_groups(const std::vector<Factory*> &factories);

private:
	const std::string name;
	const std::string short_name;
	const std::string prefix;
};

}
}

#endif /* FACTORY_HPP */
