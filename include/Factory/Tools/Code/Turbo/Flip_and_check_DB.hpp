/*!
 * \file
 * \brief Class factory::Flip_and_check_DB.
 */
#ifndef FACTORY_FLIP_AND_CHECK_DB_HPP
#define FACTORY_FLIP_AND_CHECK_DB_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check_DB.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Flip_and_check_DB_name;
extern const std::string Flip_and_check_DB_prefix;
class Flip_and_check_DB : public Flip_and_check
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Flip_and_check_DB(const std::string &p = Flip_and_check_DB_prefix);
	virtual ~Flip_and_check_DB() = default;
	Flip_and_check_DB* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Flip_and_check_DB<B,Q>* build(const module::CRC<B> &crc) const;
};
}
}

#endif /* FACTORY_FLIP_AND_CHECK_DB_HPP */
