/*!
 * \file
 * \brief Class factory::Codec_SIHO_HIHO.
 */
#ifndef FACTORY_CODEC_SIHO_HIHO_HPP_
#define FACTORY_CODEC_SIHO_HIHO_HPP_

#include <string>

#include "Tools/Codec/Codec_SIHO_HIHO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"
#include "Factory/Tools/Codec/Codec_HIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_SIHO_HIHO_name;
extern const std::string Codec_SIHO_HIHO_prefix;
class Codec_SIHO_HIHO : virtual public Codec_SIHO, virtual public Codec_HIHO
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Codec_SIHO_HIHO(const std::string &p = Codec_SIHO_HIHO_prefix);
	virtual ~Codec_SIHO_HIHO() = default;
	virtual Codec_SIHO_HIHO* clone() const = 0;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_SIHO_HIHO<B,Q>* build(module::CRC<B>* crc = nullptr) const;

protected:
	Codec_SIHO_HIHO(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_CODEC_SIHO_HIHO_HPP_ */
