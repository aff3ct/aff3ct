#ifndef FACTORY_CODEC_SIHO_HPP_
#define FACTORY_CODEC_SIHO_HPP_

#include <string>

#include "Module/Codec/Codec_SIHO.hpp"
#include "Module/CRC/CRC.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_SIHO : Codec
{
	static const std::string name;
	static const std::string prefix;

	class parameters : virtual public Codec::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Codec_SIHO::prefix);
		virtual ~parameters();
		virtual Codec_SIHO::parameters* clone() const = 0;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_SIHO<B,Q>* build(module::CRC<B>* crc = nullptr) const;

	protected:
		parameters(const std::string n, const std::string p);
	};

	template <typename B = int, typename Q = float>
	static module::Codec_SIHO<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_SIHO_HPP_ */
