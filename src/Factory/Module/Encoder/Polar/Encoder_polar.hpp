#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_polar_name;
extern const std::string Encoder_polar_prefix;
struct Encoder_polar : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_polar_prefix);
		virtual ~parameters() = default;
		Encoder_polar::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_polar<B>* build(const std::vector<bool> &frozen_bits) const;
	};

	template <typename B = int>
	static module::Encoder_polar<B>* build(const parameters &params, const std::vector<bool> &frozen_bits);
};
}
}

#endif /* FACTORY_ENCODER_POLAR_HPP */
