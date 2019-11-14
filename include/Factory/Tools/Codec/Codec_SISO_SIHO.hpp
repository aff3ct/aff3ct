/*!
 * \file
 * \brief Class factory::Codec_SISO_SIHO.
 */
#ifndef FACTORY_CODEC_SISO_SIHO_HPP_
#define FACTORY_CODEC_SISO_SIHO_HPP_

#include <string>

#include "Tools/Codec/Codec_SISO_SIHO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Tools/Codec/Codec_SISO.hpp"
#include "Factory/Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_SISO_SIHO_name;
extern const std::string Codec_SISO_SIHO_prefix;
class Codec_SISO_SIHO : virtual public Codec_SISO, virtual public Codec_SIHO
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Codec_SISO_SIHO(const std::string &p = Codec_SISO_SIHO_prefix);
	virtual ~Codec_SISO_SIHO() = default;
	virtual Codec_SISO_SIHO* clone() const = 0;

	// builder
	template <typename B = int, typename Q = float>
	tools::Codec_SISO_SIHO<B,Q>* build(module::CRC<B>* crc = nullptr) const;

protected:
	Codec_SISO_SIHO(const std::string &n, const std::string &p);
};
}
}

#endif /* FACTORY_CODEC_SISO_SIHO_HPP_ */
