/*!
 * \file
 * \brief Class factory::Encoder_repetition.
 */
#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_repetition_name;
extern const std::string Encoder_repetition_prefix;
class Encoder_repetition : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// optional
	bool buffered = true;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_repetition(const std::string &p = Encoder_repetition_prefix);
	virtual ~Encoder_repetition() = default;
	Encoder_repetition* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_repetition_sys<B>* build() const;
};
}
}

#endif /* FACTORY_ENCODER_REPETITION_HPP */
