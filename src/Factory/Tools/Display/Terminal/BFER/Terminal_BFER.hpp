#ifndef FACTORY_TERMINAL_BFER_HPP_
#define FACTORY_TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "Module/Monitor/BFER/Monitor_BFER.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Terminal_BFER_name;
extern const std::string Terminal_BFER_prefix;
struct Terminal_BFER : Terminal
{
	class parameters : public Terminal::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string type = "STD";

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Terminal_BFER_prefix);
		virtual ~parameters();
		Terminal_BFER::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float>
		tools::Terminal_BFER<B,R>* build(const module::Monitor_BFER<B,R> &monitor,
		                                 bool display_mutinfo = false, bool display_bfer = true) const;
	};

	template <typename B = int, typename R = float>
	static tools::Terminal_BFER<B,R>* build(const parameters &params, const module::Monitor_BFER<B,R> &monitor,
	                                        bool display_mutinfo = false, bool display_bfer = true);
};
}
}

#endif /* FACTORY_TERMINAL_HPP_ */
