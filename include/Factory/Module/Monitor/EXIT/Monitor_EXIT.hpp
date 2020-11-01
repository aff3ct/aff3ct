/*!
 * \file
 * \brief Class factory::Monitor_EXIT.
 */
#ifndef FACTORY_MONITOR_EXIT_HPP
#define FACTORY_MONITOR_EXIT_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_EXIT_name;
extern const std::string Monitor_EXIT_prefix;
class Monitor_EXIT : public Monitor
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         size     = 0;

	// optional parameters
	std::string type     = "STD";
	int         n_trials = 200;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Monitor_EXIT(const std::string &p = Monitor_EXIT_prefix);
	virtual ~Monitor_EXIT() = default;
	Monitor_EXIT* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename R = float>
	module::Monitor_EXIT<B,R>* build() const;
};
}
}

#endif /* FACTORY_MONITOR_EXIT_HPP */
