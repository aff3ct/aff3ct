#ifndef FACTORY_MONITOR_BFER_HPP
#define FACTORY_MONITOR_BFER_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_BFER_name;
extern const std::string Monitor_BFER_prefix;
struct Monitor_BFER : public Monitor
{
	class parameters : public Monitor::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K = 0;

		// optional parameters
		std::string type           = "STD";
		std::string err_hist_path  = "hist";
		int         err_hist       = -1;
		int         n_frame_errors = 100;
		int         max_frame      = 0;
		int         n_frames       = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Monitor_BFER_prefix);
		virtual ~parameters() = default;
		Monitor_BFER::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Monitor_BFER<B>* build(bool count_unknown_values = false) const;
	};

	template <typename B = int>
	static module::Monitor_BFER<B>* build(const parameters& params, bool count_unknown_values = false);
};
}
}

#endif /* FACTORY_MONITOR_BFER_HPP */
