/*!
 * \file
 * \brief Class factory::Encoder_RSC.
 */
#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RSC_name;
extern const std::string Encoder_RSC_prefix;
class Encoder_RSC : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional
	bool             buffered = true;
	std::string      standard = "LTE";
	std::vector<int> poly     = {013, 015};

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_RSC(const std::string &p = Encoder_RSC_prefix);
	virtual ~Encoder_RSC() = default;
	Encoder_RSC* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_RSC_sys<B>* build(std::ostream &stream = std::cout) const;
};
}
}

#endif /* FACTORY_ENCODER_RSC_HPP */
