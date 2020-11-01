/*!
 * \file
 * \brief Class factory::Monitor_BFER.
 */
#ifndef FACTORY_MONITOR_BFER_HPP
#define FACTORY_MONITOR_BFER_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Monitor_BFER_name;
extern const std::string Monitor_BFER_prefix;
class Monitor_BFER : public Monitor
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         K = 0;

	// optional parameters
	std::string type           = "STD";
	std::string err_hist_path  = "hist";
	int         err_hist       = -1;
	int         n_frame_errors = 100;
	int         max_frame      = 0;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Monitor_BFER(const std::string &p = Monitor_BFER_prefix);
	virtual ~Monitor_BFER() = default;
	Monitor_BFER* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Monitor_BFER<B>* build(bool count_unknown_values = false) const;
};
}
}

#endif /* FACTORY_MONITOR_BFER_HPP */
