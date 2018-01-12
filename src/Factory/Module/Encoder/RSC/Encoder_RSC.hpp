#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <string>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_RSC : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		bool             buffered = true;
		std::string      standard = "LTE";
		std::vector<int> poly     = {013, 015};

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Encoder_RSC::prefix);
		virtual ~parameters();
		Encoder_RSC::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
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
