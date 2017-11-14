#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>
#include <sstream>

#include "Module/CRC/CRC.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct CRC : public Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K             = 0;

		// optional parameters
		std::string type          = "FAST";
		std::string poly          = "";
		int         size          = 0;
		int         n_frames      = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = CRC::prefix);
		virtual ~parameters();
		CRC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
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
