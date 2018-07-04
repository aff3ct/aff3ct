#ifndef FACTORY_CODEC_RA_HPP
#define FACTORY_CODEC_RA_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/RA/Encoder_RA.hpp"
#include "Factory/Module/Decoder/RA/Decoder_RA.hpp"

#include "Module/Codec/RA/Codec_RA.hpp"

#include "../Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Codec_RA_name;
extern const std::string Codec_RA_prefix;
struct Codec_RA : public Codec_SIHO
{
	class parameters : public Codec_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_RA::parameters *enc;
		Decoder_RA::parameters *dec;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Codec_RA_prefix);
		virtual ~parameters();
		Codec_RA::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_RA<B,Q>* build(module::CRC<B> *crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_RA<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_RA_HPP */
