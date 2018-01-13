#ifndef FACTORY_ENCODER_BCH_HPP
#define FACTORY_ENCODER_BCH_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"
#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"

#include "../Encoder.hpp"

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
		virtual ~parameters();
		Encoder_BCH::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder<B>* build(const tools::BCH_polynomial_generator &GF) const;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters &params, const tools::BCH_polynomial_generator &GF);
};
}
}

#endif /* FACTORY_ENCODER_BCH_HPP */
