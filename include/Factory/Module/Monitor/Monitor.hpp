/*!
 * \file
 * \brief Class factory::Monitor.
 */
#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_name;
extern const std::string Monitor_prefix;
class Monitor : public Factory
{
public:
	explicit Monitor(const std::string &p = Monitor_prefix);
	virtual ~Monitor() = default;
	virtual Monitor* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

protected:
	Monitor(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
