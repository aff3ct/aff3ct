#ifndef FACTORY_ENCODER_RS_HPP
#define FACTORY_ENCODER_RS_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Encoder/RS/Encoder_RS.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RS_name;
extern const std::string Encoder_RS_prefix;
struct Encoder_RS : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_RS_prefix);
		virtual ~parameters() = default;
		Encoder_RS::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_RS<B>* build(const tools::RS_polynomial_generator &GF) const;
	};

	template <typename B = int>
	static module::Encoder_RS<B>* build(const parameters &params, const tools::RS_polynomial_generator &GF);
};
}
}

#endif /* FACTORY_ENCODER_RS_HPP */
