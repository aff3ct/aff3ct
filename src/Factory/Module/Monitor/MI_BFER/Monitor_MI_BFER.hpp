#ifndef FACTORY_MONITOR_MI_BFER_HPP
#define FACTORY_MONITOR_MI_BFER_HPP

#include <string>

#include "Module/Monitor/MI_BFER/Monitor_MI_BFER.hpp"

#include "../MI/Monitor_MI.hpp"
#include "../BFER/Monitor_BFER.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_MI_BFER_name;
extern const std::string Monitor_MI_BFER_prefix;
struct Monitor_MI_BFER : public Monitor_MI, public Monitor_BFER
{
	class parameters : public Monitor_MI::parameters, public Monitor_BFER::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K = 0;
		int         N = 0;

		// optional parameters
		std::string type    = "STD";
		bool        mutinfo = false;
		int         n_frames = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Monitor_MI_BFER_prefix);
		virtual ~parameters();
		Monitor_MI_BFER::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float>
		module::Monitor_MI_BFER<B,R>* build(bool count_unknown_values = false) const;
	};

	template <typename B = int, typename R = float>
	static module::Monitor_MI_BFER<B,R>* build(const parameters& params, bool count_unknown_values = false);
};
}
}

#endif /* FACTORY_MONITOR_MI_BFER_HPP */
