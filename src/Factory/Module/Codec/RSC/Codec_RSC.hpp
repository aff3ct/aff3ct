#ifndef FACTORY_CODEC_RSC_HPP
#define FACTORY_CODEC_RSC_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"
#include "Factory/Module/Decoder/RSC/Decoder_RSC.hpp"

#include "Module/Codec/RSC/Codec_RSC.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_RSC : public Codec_SISO_SIHO
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_RSC::parameters *enc;
		Decoder_RSC::parameters *dec;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Codec_RSC::prefix);
		virtual ~parameters();
		Codec_RSC::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_RSC<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_RSC<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_RSC_HPP */
