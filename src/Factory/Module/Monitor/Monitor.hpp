#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>

#include "Module/Monitor/Monitor.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Monitor : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Monitor::prefix);
		virtual ~parameters();
		virtual Monitor::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		virtual void store          (const tools::Argument_map_value &vals                                 );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string n, const std::string p);
	};
};
}
}

#endif /* FACTORY_MONITOR_HPP */
