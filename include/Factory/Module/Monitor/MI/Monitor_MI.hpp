/*!
 * \file
 * \brief Class factory::Monitor_MI.
 */
#ifndef FACTORY_MONITOR_MI_HPP
#define FACTORY_MONITOR_MI_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_MI_name;
extern const std::string Monitor_MI_prefix;
class Monitor_MI : public Monitor
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         N = 0;

	// optional parameters
	std::string type     = "STD";
	int         n_trials = 200;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Monitor_MI(const std::string &p = Monitor_MI_prefix);
	virtual ~Monitor_MI() = default;
	Monitor_MI* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename R = float>
	module::Monitor_MI<B,R>* build() const;
};
}
}

#endif /* FACTORY_MONITOR_MI_HPP */
