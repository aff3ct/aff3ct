#ifndef FACTORY_TERMINAL_HPP_
#define FACTORY_TERMINAL_HPP_

#include <chrono>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Terminal_name;
extern const std::string Terminal_prefix;
struct Terminal : Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string               type      = "STD";
		std::chrono::milliseconds frequency = std::chrono::milliseconds(500);
		bool                      disabled  = false;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Terminal_prefix);
		virtual ~parameters() = default;
		virtual Terminal::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		tools::Terminal* build(const std::vector<std::unique_ptr<tools::Reporter>> &reporters) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	static tools::Terminal* build(const parameters &params, const std::vector<std::unique_ptr<tools::Reporter>> &reporters);
};
}
}

#endif /* FACTORY_TERMINAL_HPP_ */
