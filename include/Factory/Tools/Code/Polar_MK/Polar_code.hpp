/*!
 * \file
 * \brief Class factory::Polar_code.
 */
#ifndef FACTORY_POLAR_CODE_HPP
#define FACTORY_POLAR_CODE_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Polar_code_name;
extern const std::string Polar_code_prefix;
class Polar_code : public Factory
{
public:
	// ------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int N_cw = -1;

	// optional parameters
	std::vector<std::vector<bool>> kernel_matrix = {{1,0},{1,1}};
	std::string                    code_path;

	// ---------------------------------------------------------------------------------------------------- METHODS
	explicit Polar_code(const std::string &p = Polar_code_prefix);
	virtual ~Polar_code() = default;
	Polar_code* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	tools::Polar_code* build() const;
};
}
}

#endif /* FACTORY_POLAR_CODE_HPP */
