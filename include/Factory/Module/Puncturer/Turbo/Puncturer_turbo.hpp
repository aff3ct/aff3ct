/*!
 * \file
 * \brief Class factory::Puncturer_turbo.
 */
#ifndef FACTORY_PUNCTURER_TURBO_HPP
#define FACTORY_PUNCTURER_TURBO_HPP

#include <vector>
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
extern const std::string Puncturer_turbo_name;
extern const std::string Puncturer_turbo_prefix;
class Puncturer_turbo : public Puncturer
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::vector<std::vector<bool>> pattern     = {{1,1,1},{1,1,1},{1,1,1}};
	bool                           buffered    = true;
	int                            tail_length = 0;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Puncturer_turbo(const std::string &p = Puncturer_turbo_prefix);
	virtual ~Puncturer_turbo() = default;
	Puncturer_turbo* clone() const;

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

#endif /* FACTORY_PUNCTURER_TURBO_HPP */
