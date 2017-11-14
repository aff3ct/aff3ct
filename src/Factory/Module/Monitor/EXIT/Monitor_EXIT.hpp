#ifndef FACTORY_MONITOR_EXIT_HPP
#define FACTORY_MONITOR_EXIT_HPP

#include <string>

#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

#include "../Monitor.hpp"

namespace aff3ct
{
namespace factory
{
struct Monitor_EXIT : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Monitor::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         size     = 0;

		// optional parameters
		std::string type     = "STD";
		int         n_trials = 200;
		int         n_frames = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Monitor_EXIT::prefix);
		virtual ~parameters();
		Monitor_EXIT::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float>
		module::Monitor_EXIT<B,R>* build() const;
	};

	template <typename B = int, typename R = float>
	static module::Monitor_EXIT<B,R>* build(const parameters& params);
};
}
}

#endif /* FACTORY_MONITOR_EXIT_HPP */
