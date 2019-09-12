#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RSC_name;
extern const std::string Encoder_RSC_prefix;
struct Encoder_RSC : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		bool             buffered = true;
		std::string      standard = "LTE";
		std::vector<int> poly     = {013, 015};

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_RSC_prefix);
		virtual ~parameters() = default;
		Encoder_RSC::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_RSC_sys<B>* build(std::ostream &stream = std::cout) const;
	};

	template <typename B = int>
	static module::Encoder_RSC_sys<B>* build(const parameters &params, std::ostream &stream = std::cout);
};
}
}

#endif /* FACTORY_ENCODER_RSC_HPP */
