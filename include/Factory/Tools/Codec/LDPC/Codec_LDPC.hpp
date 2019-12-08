/*!
 * \file
 * \brief Class factory::Codec_LDPC.
 */
#ifndef FACTORY_CODEC_LDPC_HPP
#define FACTORY_CODEC_LDPC_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/LDPC/Codec_LDPC.hpp"
#include "Factory/Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_LDPC_name;
extern const std::string Codec_LDPC_prefix;
class Codec_LDPC : public Codec_SISO
{
public:
	explicit Codec_LDPC(const std::string &p = Codec_LDPC_prefix);
	virtual ~Codec_LDPC() = default;
	Codec_LDPC* clone() const;
	void enable_puncturer();

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_LDPC<B,Q>* build(const module::CRC<B>* crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_LDPC_HPP */
