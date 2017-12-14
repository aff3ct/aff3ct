#ifndef FACTORY_CODEC_LDPC_HPP
#define FACTORY_CODEC_LDPC_HPP

#include <string>
#include <cmath>

#include "Factory/Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Factory/Module/Puncturer/LDPC/Puncturer_LDPC.hpp"
#include "Factory/Module/Decoder/LDPC/Decoder_LDPC.hpp"

#include "Module/Codec/LDPC/Codec_LDPC.hpp"

#include "../Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace factory
{
struct Codec_LDPC : public Codec_SISO_SIHO
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Codec_SISO_SIHO::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Encoder_LDPC  ::parameters *enc;
		Decoder_LDPC  ::parameters *dec;
		Puncturer_LDPC::parameters *pct;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Codec_LDPC::prefix);
		virtual ~parameters();
		Codec_LDPC::parameters* clone() const;
		void enable_puncturer();

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Codec_LDPC<B,Q>* build(module::CRC<B>* crc = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Codec_LDPC<B,Q>* build(const parameters &params, module::CRC<B>* crc = nullptr);
};
}
}

#endif /* FACTORY_CODEC_LDPC_HPP */
