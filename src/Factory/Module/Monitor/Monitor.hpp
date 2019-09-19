#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_name;
extern const std::string Monitor_prefix;
struct Monitor : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		explicit parameters(const std::string &p = Monitor_prefix);
		virtual ~parameters() = default;
		virtual Monitor::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};
};
}
}

#endif /* FACTORY_MONITOR_HPP */
