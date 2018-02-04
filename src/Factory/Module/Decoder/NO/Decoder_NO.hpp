#ifndef FACTORY_DECODER_NO_HPP
#define FACTORY_DECODER_NO_HPP

#include <string>

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Encoder/Encoder.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_NO_name;
extern const std::string Decoder_NO_prefix;
struct Decoder_NO : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_NO_prefix);
		virtual ~parameters();
		Decoder_NO::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(module::Encoder<B> *encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(module::Encoder<B> *encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters &params, module::Encoder<B> *encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params, module::Encoder<B> *encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_NO_HPP */
