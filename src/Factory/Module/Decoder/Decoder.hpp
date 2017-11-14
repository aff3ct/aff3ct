#ifndef FACTORY_DECODER_HPP_
#define FACTORY_DECODER_HPP_

#include <string>

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder : Factory
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         K           = 0;
		int         N_cw        = 0;

		// optional parameters
		std::string type        = "";
		std::string implem      = "";
		bool        systematic  = true;
		int         n_frames    = 1;
		int         tail_length = 0;

		// deduced parameters
		float       R           = -1.f;

		// ---------------------------------------------------------------------------------------------------- METHODS
		virtual ~parameters();
		virtual Decoder::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		virtual void store          (const tools::Argument_map_value &vals                                 );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string n, const std::string p);
	};
};
}
}

#endif /* FACTORY_DECODER_HPP_ */
