#ifndef FACTORY_CODEC_TURBO_PRODUCT_CODE_HPP
#define FACTORY_CODEC_TURBO_PRODUCT_CODE_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/Turbo_product_code/Encoder_turbo_product_code.hpp"
#include "Factory/Module/Decoder/Turbo_product_code/Decoder_turbo_product_code.hpp"

#include "Module/Codec/Turbo_product_code/Codec_turbo_product_code.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_turbo_product_code_name;
extern const std::string Codec_turbo_product_code_prefix;
struct Codec_turbo_product_code : public Codec_SISO_SIHO
{
	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_turbo_product_code::parameters *enc;
		Decoder_turbo_product_code::parameters *dec;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_turbo_product_code_prefix);
		virtual ~parameters();
		Codec_turbo_product_code::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_turbo_product_code<B,Q>* build(module::CRC<B> *crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_turbo_product_code<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_TURBO_PRODUCT_CODE_HPP */
