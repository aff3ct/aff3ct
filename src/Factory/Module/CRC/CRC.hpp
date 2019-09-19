#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string CRC_name;
extern const std::string CRC_prefix;
struct CRC : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K             = 0;

		// optional parameters
		std::string type          = "NO"; // "32-GZIP"; // type is the polynomial
		std::string implem        = "FAST";
		int         size          = 0;
		int         n_frames      = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = CRC_prefix);
		virtual ~parameters() = default;
		CRC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::CRC<B>* build() const;
	};

	template <typename B = int>
	static module::CRC<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_CRC_HPP */
