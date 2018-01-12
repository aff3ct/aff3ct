#ifndef FACTORY_CODEC_BCH_HPP
#define FACTORY_CODEC_BCH_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Module/Codec/BCH/Codec_BCH.hpp"

#include "../Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_BCH : public Codec_SIHO
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Codec_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_BCH::parameters *enc;
		Decoder_BCH::parameters *dec;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Codec_BCH::prefix);
		virtual ~parameters();
		Codec_BCH::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_BCH<B,Q>* build(module::CRC<B> *crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_BCH<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_BCH_HPP */
