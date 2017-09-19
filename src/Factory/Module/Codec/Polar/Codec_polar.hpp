#ifndef FACTORY_CODEC_POLAR_HPP
#define FACTORY_CODEC_POLAR_HPP

#include <string>
#include <cmath>

#include "Factory/Tools/Code/Polar/Frozenbits_generator.hpp"
#include "Factory/Module/Encoder/Polar/Encoder_polar.hpp"
#include "Factory/Module/Decoder/Polar/Decoder_polar.hpp"
#include "Factory/Module/Puncturer/Polar/Puncturer_polar.hpp"

#include "Module/CRC/CRC.hpp"
#include "Module/Codec/Polar/Codec_polar.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_polar : public Codec_SISO_SIHO
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_polar       ::parameters *enc;
		Frozenbits_generator::parameters *fbg;
		Decoder_polar       ::parameters *dec;
		Puncturer_polar     ::parameters *pct;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Codec_polar::prefix);
		virtual ~parameters();
		Codec_polar::parameters* clone() const;
		void enable_puncturer();

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_polar<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_polar<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_POLAR_HPP */
