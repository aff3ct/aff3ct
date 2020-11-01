/*!
 * \file
 * \brief Class factory::CRC.
 */
#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string CRC_name;
extern const std::string CRC_prefix;
class CRC : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         K             = 0;

	// optional parameters
	std::string type          = "NO"; // "32-GZIP"; // type is the polynomial
	std::string implem        = "FAST";
	int         size          = 0;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit CRC(const std::string &p = CRC_prefix);
	virtual ~CRC() = default;
	CRC* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::CRC<B>* build() const;
};
}
}

#endif /* FACTORY_CRC_HPP */
