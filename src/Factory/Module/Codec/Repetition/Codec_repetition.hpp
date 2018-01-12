#ifndef FACTORY_CODEC_REPETITION_HPP
#define FACTORY_CODEC_REPETITION_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/Repetition/Encoder_repetition.hpp"
#include "Factory/Module/Decoder/Repetition/Decoder_repetition.hpp"

#include "Module/Codec/Repetition/Codec_repetition.hpp"

#include "../Codec_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_repetition : public Codec_SIHO
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Codec_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_repetition::parameters *enc;
		Decoder_repetition::parameters *dec;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Codec_repetition::prefix);
		virtual ~parameters();
		Codec_repetition::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_repetition<B,Q>* build(module::CRC<B> *crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_repetition<B,Q>* build(const parameters &params, module::CRC<B> *crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_REPETITION_HPP */
