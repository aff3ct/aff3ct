/*!
 * \file
 * \brief Class factory::Codec_repetition.
 */
#ifndef FACTORY_CODEC_REPETITION_HPP
#define FACTORY_CODEC_REPETITION_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/Repetition/Codec_repetition.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_repetition_name;
extern const std::string Codec_repetition_prefix;
class Codec_repetition : public Codec_SIHO
{
public:
	explicit Codec_repetition(const std::string &p = Codec_repetition_prefix);
	virtual ~Codec_repetition() = default;
	Codec_repetition* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_repetition<B,Q>* build(const module::CRC<B> *crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_REPETITION_HPP */
