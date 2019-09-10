#ifndef FACTORY_POLAR_CODE_HPP
#define FACTORY_POLAR_CODE_HPP

#include "Tools/Code/Polar/Polar_code.hpp"

#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Polar_code_name;
extern const std::string Polar_code_prefix;
struct Polar_code : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int N_cw = -1;

		// optional parameters
		std::vector<std::vector<bool>> kernel_matrix = {{1,0},{1,1}};
		std::string                    code_path;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Polar_code_prefix);
		virtual ~parameters() = default;
		Polar_code::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		tools::Polar_code* build() const;
	};

	static tools::Polar_code* build(const parameters &params);
};
}
}

#endif /* FACTORY_POLAR_CODE_HPP */
