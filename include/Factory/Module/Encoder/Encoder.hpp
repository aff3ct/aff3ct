/*!
 * \file
 * \brief Class factory::Encoder.
 */
#ifndef FACTORY_ENCODER_HPP
#define FACTORY_ENCODER_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_name;
extern const std::string Encoder_prefix;
class Encoder : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	int         K           = 0;
	int         N_cw        = 0;

	// optional parameters
	std::string type        = "AZCW";
	std::string path        = "";
	bool        systematic  = true;
	int         seed        = 0;
	int         tail_length = 0;
	int         start_idx   = 0;

	// deduced parameters
	float       R           = -1.f;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder(const std::string &p = Encoder_prefix);
	virtual ~Encoder() = default;
	virtual Encoder* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder<B>* build() const;

protected:
	Encoder(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_ENCODER_HPP */
