#ifndef FACTORY_PUNCTURER_LDPC_HPP
#define FACTORY_PUNCTURER_LDPC_HPP

#include <string>

#include "Module/Puncturer/Puncturer.hpp"

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
struct Puncturer_LDPC : public Puncturer
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Puncturer::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		std::vector<bool> pattern;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Puncturer_LDPC::prefix);
		virtual ~parameters();
		Puncturer_LDPC::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Puncturer<B,Q>* build() const;
	};

	template <typename B = int, typename Q = float>
	static module::Puncturer<B,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_PUNCTURER_LDPC_HPP */
