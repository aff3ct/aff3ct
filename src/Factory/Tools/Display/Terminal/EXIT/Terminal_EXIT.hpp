#ifndef FACTORY_TERMINAL_EXIT_HPP_
#define FACTORY_TERMINAL_EXIT_HPP_

#include <string>
#include <chrono>

#include "Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Terminal_EXIT_name;
extern const std::string Terminal_EXIT_prefix;
struct Terminal_EXIT : Terminal
{
	class parameters : public Terminal::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::string type = "STD";

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Terminal_EXIT_prefix);
		virtual ~parameters();
		Terminal_EXIT::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float>
		tools::Terminal_EXIT<B,R>* build(const module::Monitor_EXIT<B,R> &monitor) const;
	};

	template <typename B = int, typename R = float>
	static tools::Terminal_EXIT<B,R>* build(const parameters &params, const module::Monitor_EXIT<B,R> &monitor);
};
}
}

#endif /* FACTORY_TERMINAL_EXIT_HPP_ */
