/*!
 * \file
 * \brief Class factory::Encoder_polar_MK.
 */
#ifndef FACTORY_ENCODER_POLAR_MK_HPP
#define FACTORY_ENCODER_POLAR_MK_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Code/Polar/Polar_code.hpp"
#include "Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_polar_MK_name;
extern const std::string Encoder_polar_MK_prefix;
class Encoder_polar_MK : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_polar_MK(const std::string &p = Encoder_polar_MK_prefix);
	virtual ~Encoder_polar_MK() = default;
	Encoder_polar_MK* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_polar_MK<B>* build(const tools::Polar_code &code, const std::vector<bool> &frozen_bits) const;
};
}
}

#endif /* FACTORY_ENCODER_POLAR_MK_HPP */
