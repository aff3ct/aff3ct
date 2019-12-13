/*!
 * \file
 * \brief Class factory::Codec_polar_MK.
 */
#ifndef FACTORY_CODEC_POLAR_MK_HPP
#define FACTORY_CODEC_POLAR_MK_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/Polar_MK/Codec_polar_MK.hpp"
#include "Factory/Tools/Code/Polar_MK/Frozenbits_generator_MK.hpp"
#include "Factory/Tools/Code/Polar_MK/Polar_code.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_polar_MK_name;
extern const std::string Codec_polar_MK_prefix;
class Codec_polar_MK : public Codec_SIHO
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// depending parameters
	tools::auto_cloned_unique_ptr<Polar_code             > pc;
	tools::auto_cloned_unique_ptr<Frozenbits_generator_MK> fbg;

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Codec_polar_MK(const std::string &p = Codec_polar_MK_prefix);
	virtual ~Codec_polar_MK() = default;
	Codec_polar_MK* clone() const;

	virtual std::vector<std::string> get_names      () const;
	virtual std::vector<std::string> get_short_names() const;
	virtual std::vector<std::string> get_prefixes   () const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_polar_MK<B,Q>* build(const module::CRC<B>* crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_POLAR_MK_HPP */
