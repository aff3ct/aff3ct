/*!
 * \file
 * \brief Class factory::Terminal.
 */
#ifndef FACTORY_TERMINAL_HPP_
#define FACTORY_TERMINAL_HPP_

#include <chrono>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Terminal_name;
extern const std::string Terminal_prefix;
class Terminal : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::string               type      = "STD";
	std::chrono::milliseconds frequency = std::chrono::milliseconds(500);
	bool                      disabled  = false;

	// -------------------------------------------------------------------------------------------------------- METHODS
	Terminal(const std::string &p = Terminal_prefix);
	virtual ~Terminal() = default;
	virtual Terminal* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	tools::Terminal* build(const std::vector<std::unique_ptr<tools::Reporter>> &reporters) const;

protected:
	Terminal(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_TERMINAL_HPP_ */
