#ifndef FACTORY_CODEC_SIHO_HIHO_HPP_
#define FACTORY_CODEC_SIHO_HIHO_HPP_

#include <string>

#include "Module/Codec/Codec_SIHO_HIHO.hpp"
#include "Module/CRC/CRC.hpp"
#include "Factory/Module/Codec/Codec_SIHO.hpp"
#include "Factory/Module/Codec/Codec_HIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_SIHO_HIHO_name;
extern const std::string Codec_SIHO_HIHO_prefix;
struct Codec_SIHO_HIHO : Codec
{
	class parameters : virtual public Codec_SIHO::parameters, virtual public Codec_HIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_SIHO_HIHO_prefix);
		virtual ~parameters() = default;
		virtual Codec_SIHO_HIHO::parameters* clone() const = 0;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_SIHO_HIHO<B,Q>* build(module::CRC<B>* crc = nullptr) const;

	protected:
		parameters(const std::string &n, const std::string &p);
	};

	template <typename B = int, typename Q = float>
	static module::Codec_SIHO_HIHO<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_SIHO_HIHO_HPP_ */
