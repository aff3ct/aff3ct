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
extern const std::string Launcher_name;
extern const std::string Launcher_prefix;
struct Launcher : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		std::string cde_type;

		// optional parameters
		std::string sim_type         = "BFER";
		int         sim_prec         = 32;
		bool        display_help     = false;
		bool        display_adv_help = false;
		bool        display_version  = false;
		bool        display_legend   = true;
		bool        display_keys     = false;
		bool        full_legend      = false;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Launcher_prefix);
		virtual ~parameters() = default;
		virtual Launcher::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
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
