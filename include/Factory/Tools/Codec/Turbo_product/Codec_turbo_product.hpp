/*!
 * \file
 * \brief Class factory::Codec_turbo_product.
 */
#ifndef FACTORY_CODEC_TURBO_PRODUCT_HPP
#define FACTORY_CODEC_TURBO_PRODUCT_HPP

#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Codec/Turbo_product/Codec_turbo_product.hpp"
#include "Factory/Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_turbo_product_name;
extern const std::string Codec_turbo_product_prefix;
class Codec_turbo_product : public Codec_SISO
{
public:
	explicit Codec_turbo_product(const std::string &p = Codec_turbo_product_prefix);
	virtual ~Codec_turbo_product() = default;
	Codec_turbo_product* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_turbo_product<B,Q>* build(const module::CRC<B> *crc = nullptr) const;
};
}
}

#endif /* FACTORY_CODEC_TURBO_PRODUCT_HPP */
