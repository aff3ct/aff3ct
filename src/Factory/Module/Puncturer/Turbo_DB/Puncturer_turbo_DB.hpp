#ifndef FACTORY_PUNCTURER_TURBO_DB_HPP
#define FACTORY_PUNCTURER_TURBO_DB_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Puncturer_turbo_DB_name;
extern const std::string Puncturer_turbo_DB_prefix;
struct Puncturer_turbo_DB : public Puncturer
{
	class parameters : public Puncturer::parameters
	{
	public:
		explicit parameters(const std::string &p = Puncturer_turbo_DB_prefix);
		virtual ~parameters() = default;
		Puncturer_turbo_DB::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
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

#endif /* FACTORY_PUNCTURER_TURBO_DB_HPP */
