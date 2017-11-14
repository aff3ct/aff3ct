#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_repetition : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		bool buffered = true;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Encoder_repetition::prefix);
		virtual ~parameters();
		Encoder_repetition::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder<B>* build() const;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_ENCODER_REPETITION_HPP */
