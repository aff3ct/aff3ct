/*!
 * \file
 * \brief Class factory::Frozenbits_generator_MK.
 */
#ifndef FACTORY_FROZENBITS_GENERATOR_MK_HPP
#define FACTORY_FROZENBITS_GENERATOR_MK_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Frozenbits_generator_MK_name;
extern const std::string Frozenbits_generator_MK_prefix;
class Frozenbits_generator_MK : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         K                  = -1;
	int         N_cw               = -1;

	// optional parameters
	std::string type               = "GA";
	std::string path_fb            = "../conf/cde/awgn_polar_codes/TV";
	std::string dump_channels_path = "";
	float       noise              = -1.f;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Frozenbits_generator_MK(const std::string &p = Frozenbits_generator_MK_prefix);
	virtual ~Frozenbits_generator_MK() = default;
	Frozenbits_generator_MK* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	tools::Frozenbits_generator* build(const tools::Polar_code &pc) const;
};
}
}

#endif /* FACTORY_FROZENBITS_GENERATOR_MK_HPP */
