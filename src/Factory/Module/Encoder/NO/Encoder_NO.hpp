#ifndef FACTORY_ENCODER_NO_HPP
#define FACTORY_ENCODER_NO_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_NO_name;
extern const std::string Encoder_NO_prefix;
struct Encoder_NO : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_NO_prefix);
		virtual ~parameters() = default;
		Encoder_NO::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_NO<B>* build() const;
	};

	template <typename B = int>
	static module::Encoder_NO<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_ENCODER_NO_HPP */
