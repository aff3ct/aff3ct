#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Quantizer/Quantizer.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Quantizer_name;
extern const std::string Quantizer_prefix;
struct Quantizer : public Factory
{
	class parameters : public Factory::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		int         size       = 0;

		// optional parameters
		std::string type       = "POW2";
		std::string implem     = "STD";
		float       range      = 0.f;
		int         n_bits     = 8;
		int         n_decimals = 3;
		int         n_frames   = 1;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Quantizer_prefix);
		virtual ~parameters() = default;
		Quantizer::parameters* clone() const;

		// parameters construction
		virtual void get_description(tools::Argument_map_info &args) const;
		virtual void store          (const tools::Argument_map_value &vals);
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename R = float, typename Q = R>
		module::Quantizer<R,Q>* build() const;
	};

	template <typename R = float, typename Q = R>
	static module::Quantizer<R,Q>* build(const parameters& params);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
