#ifndef FACTORY_MONITOR_BFER_HPP
#define FACTORY_MONITOR_BFER_HPP

#include <string>

#include "Module/Monitor/BFER/Monitor_BFER.hpp"

#include "../Monitor.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_BFER_name;
extern const std::string Monitor_BFER_prefix;
struct Monitor_BFER : public Factory
{
	class parameters : public Monitor::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K = 0;
		int         N = 0;

		// optional parameters
		std::string type           = "STD";
		std::string err_hist_path  = "hist";
		int         err_hist       = -1;
		int         n_frame_errors = 100;
		int         n_frames       = 1;
		bool        mutinfo        = false;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Monitor_BFER_prefix);
		virtual ~parameters();
		Monitor_BFER::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float>
		module::Monitor_BFER<B,R>* build() const;
	};

	template <typename B = int, typename R = float>
	static module::Monitor_BFER<B,R>* build(const parameters& params);
};
}
}

#endif /* FACTORY_MONITOR_BFER_HPP */
