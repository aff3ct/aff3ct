#ifndef FACTORY_CODEC_SISO_SIHO_HPP_
#define FACTORY_CODEC_SISO_SIHO_HPP_

#include <string>

#include "Module/Codec/Codec_SISO_SIHO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Module/Codec/Codec_SISO.hpp"
#include "Factory/Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_SISO_SIHO_name;
extern const std::string Codec_SISO_SIHO_prefix;
struct Codec_SISO_SIHO : Codec
{
	class parameters : virtual public Codec_SISO::parameters, virtual public Codec_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_SISO_SIHO_prefix);
		virtual ~parameters() = default;
		virtual Codec_SISO_SIHO::parameters* clone() const = 0;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_SISO_SIHO<B,Q>* build(module::CRC<B>* crc = nullptr) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B = int, typename Q = float>
	static module::Codec_SISO_SIHO<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_SISO_SIHO_HPP_ */
