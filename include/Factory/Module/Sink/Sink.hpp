/*!
 * \file
 * \brief Class factory::Sink.
 */
#ifndef FACTORY_SINK_HPP
#define FACTORY_SINK_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Sink/Sink.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Sink_name;
extern const std::string Sink_prefix;
class Sink : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         K      = 0;

	// optional parameters
	std::string type   = "NO";
	std::string implem = "STD";
	std::string path   = "";

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Sink(const std::string &p = Sink_prefix);
	virtual ~Sink() = default;
	Sink* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Sink<B>* build() const;
};
}
}

#endif /* FACTORY_SINK_HPP */
