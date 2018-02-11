#ifndef FACTORY_CODEC_UNCODED_HPP
#define FACTORY_CODEC_UNCODED_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Decoder/NO/Decoder_NO.hpp"

#include "Module/Codec/Uncoded/Codec_uncoded.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_uncoded_name;
extern const std::string Codec_uncoded_prefix;
struct Codec_uncoded : public Codec_SISO_SIHO
{
	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder   ::parameters *enc;
		Decoder_NO::parameters *dec;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_uncoded_prefix);
		virtual ~parameters();
		Codec_uncoded::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_uncoded<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_uncoded<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_UNCODED_HPP */
