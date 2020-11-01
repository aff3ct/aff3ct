/*!
 * \file
 * \brief Class factory::Interleaver.
 */
#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Factory/Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Interleaver_name;
extern const std::string Interleaver_prefix;
class Interleaver : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// depending parameters
	tools::auto_cloned_unique_ptr<factory::Interleaver_core> core;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Interleaver(const std::string &p = Interleaver_prefix);
	virtual ~Interleaver() = default;
	Interleaver* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename D = int32_t>
	static module::Interleaver<D>* build(tools::Interleaver_core<>& itl_core);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
