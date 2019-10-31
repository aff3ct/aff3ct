/*!
 * \file
 * \brief Class factory::Puncturer_LDPC.
 */
#ifndef FACTORY_PUNCTURER_LDPC_HPP
#define FACTORY_PUNCTURER_LDPC_HPP

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
extern const std::string Puncturer_LDPC_name;
extern const std::string Puncturer_LDPC_prefix;
class Puncturer_LDPC : public Puncturer
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional parameters
	std::vector<bool> pattern;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Puncturer_LDPC(const std::string &p = Puncturer_LDPC_prefix);
	virtual ~Puncturer_LDPC() = default;
	Puncturer_LDPC* clone() const;

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

#endif /* FACTORY_PUNCTURER_LDPC_HPP */
