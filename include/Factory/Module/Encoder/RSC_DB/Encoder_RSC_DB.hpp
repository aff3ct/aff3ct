/*!
 * \file
 * \brief Class factory::Encoder_RSC_DB.
 */
#ifndef FACTORY_ENCODER_RSC_DB_HPP
#define FACTORY_ENCODER_RSC_DB_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RSC_DB_name;
extern const std::string Encoder_RSC_DB_prefix;
class Encoder_RSC_DB : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional
	bool        buffered = true;
	std::string standard = "DVB-RCS1";

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_RSC_DB(const std::string &p = Encoder_RSC_DB_prefix);
	virtual ~Encoder_RSC_DB() = default;
	Encoder_RSC_DB* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_RSC_DB<B>* build() const;
};
}
}

#endif /* FACTORY_ENCODER_RSC_DB_HPP */
