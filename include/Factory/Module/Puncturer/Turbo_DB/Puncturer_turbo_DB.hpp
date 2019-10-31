/*!
 * \file
 * \brief Class factory::Puncturer_turbo_DB.
 */
#ifndef FACTORY_PUNCTURER_TURBO_DB_HPP
#define FACTORY_PUNCTURER_TURBO_DB_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Puncturer_turbo_DB_name;
extern const std::string Puncturer_turbo_DB_prefix;
class Puncturer_turbo_DB : public Puncturer
{
public:
	explicit Puncturer_turbo_DB(const std::string &p = Puncturer_turbo_DB_prefix);
	virtual ~Puncturer_turbo_DB() = default;
	Puncturer_turbo_DB* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	module::Puncturer<B,Q>* build() const;
};
}
}

#endif /* FACTORY_PUNCTURER_TURBO_DB_HPP */
