#ifndef FACTORY_ENCODER_POLAR_MK_HPP
#define FACTORY_ENCODER_POLAR_MK_HPP

#include <string>
#include <vector>

#include "Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"

#include "Tools/Code/Polar/Polar_code.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_polar_MK_name;
extern const std::string Encoder_polar_MK_prefix;
struct Encoder_polar_MK : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_polar_MK_prefix);
		virtual ~parameters() = default;
		Encoder_polar_MK::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_polar_MK<B>* build(const tools::Polar_code &code,
		                                   const std::vector<bool> &frozen_bits) const;
	};

	template <typename B = int>
	static module::Encoder_polar_MK<B>* build(const parameters &params,
	                                          const tools::Polar_code &code,
	                                          const std::vector<bool> &frozen_bits);
};
}
}

#endif /* FACTORY_ENCODER_POLAR_MK_HPP */
