/*!
 * \file
 * \brief Class factory::Interleaver_core.
 */
#ifndef FACTORY_INTERLEAVER_CORE_HPP
#define FACTORY_INTERLEAVER_CORE_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Interleaver_core_name;
extern const std::string Interleaver_core_prefix;
class Interleaver_core : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         size       = 0;

	// optional parameters
	std::string type       = "RANDOM";
	std::string path       = "";
	std::string read_order = "TOP_LEFT";
	int         n_cols     = 4;          // number of columns of the columns interleaver
	int         seed       = 0;
	bool        uniform    = false;      // set at true to regenerate the interleaver at each new frame

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Interleaver_core(const std::string &p = Interleaver_core_prefix);
	virtual ~Interleaver_core() = default;
	Interleaver_core* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename T = uint32_t>
	tools::Interleaver_core<T>* build() const;
};
}
}

#endif /* FACTORY_INTERLEAVER_CORE_HPP */
