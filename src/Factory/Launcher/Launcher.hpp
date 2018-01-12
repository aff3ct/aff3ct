#ifndef FACTORY_SIMULATION_MAIN_H_
#define FACTORY_SIMULATION_MAIN_H_

#include <string>

#include "../Factory.hpp"

namespace aff3ct
{
namespace launcher
{
class Launcher;
}
}

namespace aff3ct
{
namespace factory
{
struct Launcher : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		std::string cde_type;

		// optional parameters
		std::string sim_type        = "BFER";
		int         sim_prec        = 32;
		bool        display_help    = false;
		bool        display_version = false;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Launcher::prefix);
		virtual ~parameters();
		virtual Launcher::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename R = float, typename Q = R>
		launcher::Launcher* build(const int argc, const char **argv) const;

	private:
		template <typename B = int, typename R = float, typename Q = R>
		inline launcher::Launcher* build_exit(const int argc, const char **argv) const;

	protected:
		parameters(const std::string &n, const std::string &sn, const std::string &p);
	};

	template <typename B = int, typename R = float, typename Q = R>
	static launcher::Launcher* build(const parameters &params, const int argc, const char **argv);
};
}
}

#endif /* FACTORY_SIMULATION_MAIN_H_ */
