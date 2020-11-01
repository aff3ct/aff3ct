/*!
 * \file
 * \brief Class factory::Flip_and_check.
 */
#ifndef FACTORY_FLIP_AND_CHECK_HPP
#define FACTORY_FLIP_AND_CHECK_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Flip_and_check_name;
extern const std::string Flip_and_check_prefix;
class Flip_and_check : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int  size          = -1;

	// optional parameters
	bool enable        = false;
	int  q             = 10;
	int  n_ite         = 6;
	int  ite_min       = 3;
	int  ite_max       = 10;
	int  ite_step      = 1;
	int  crc_start_ite = 2;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Flip_and_check(const std::string &p = Flip_and_check_prefix);
	virtual ~Flip_and_check() = default;
	virtual Flip_and_check* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Flip_and_check<B,Q>* build(const module::CRC<B> &crc) const;

protected:
	Flip_and_check(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_FLIP_AND_CHECK_HPP */
