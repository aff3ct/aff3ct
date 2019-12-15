/*!
 * \file
 * \brief Class factory::Noise.
 */
#ifndef FACTORY_NOISE_HPP
#define FACTORY_NOISE_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Factory/Factory.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Noise_name;
extern const std::string Noise_prefix;
class Noise : public Factory
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// required parameters
	std::vector<float> range;

	// optional parameters
	std::string type     = "EBN0";
	std::string pdf_path = "";

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Noise(const std::string &p = Noise_prefix);
	virtual ~Noise() = default;
	Noise* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename R = float>
	tools::Noise<R>* build() const;

	// builder
	template <typename R = float>
	tools::Noise<R>* build(R noise_val, R bit_rate = 1., int bps = 1, int upf = 1) const;

	template <typename R = float>
	void update(tools::Noise<R> &noise, R noise_val, R bit_rate = 1., int bps = 1, int upf = 1) const;
};
}
}

#endif /* FACTORY_NOISE_HPP */
