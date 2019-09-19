#ifndef FACTORY_ENCODER_BCH_HPP
#define FACTORY_ENCODER_BCH_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_BCH_name;
extern const std::string Encoder_BCH_prefix;
struct Encoder_BCH : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_BCH_prefix);
		virtual ~parameters() = default;
		Encoder_BCH::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_BCH<B>* build(const tools::BCH_polynomial_generator<B> &GF) const;
	};

	template <typename B = int>
	static module::Encoder_BCH<B>* build(const parameters &params, const tools::BCH_polynomial_generator<B> &GF);
};
}
}

#endif /* FACTORY_ENCODER_BCH_HPP */
