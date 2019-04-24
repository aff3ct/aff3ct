#ifndef FACTORY_FROZENBITS_GENERATOR_MK_HPP
#define FACTORY_FROZENBITS_GENERATOR_MK_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"

#include "../../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Frozenbits_generator_MK_name;
extern const std::string Frozenbits_generator_MK_prefix;
struct Frozenbits_generator_MK : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K       = -1;
		int         N_cw    = -1;

		// optional parameters
		std::string type               = "GA";
		std::string path_fb            = "../conf/cde/awgn_polar_codes/TV";
		std::string dump_channels_path = "";
		float       noise              = -1.f;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Frozenbits_generator_MK_prefix);
		virtual ~parameters() = default;
		Frozenbits_generator_MK::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		tools::Frozenbits_generator* build(const tools::Polar_code &pc) const;
	};

	static tools::Frozenbits_generator* build(const parameters &params, const tools::Polar_code &pc);
};
}
}

#endif /* FACTORY_FROZENBITS_GENERATOR_MK_HPP */
