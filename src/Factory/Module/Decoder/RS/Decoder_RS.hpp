#ifndef FACTORY_DECODER_RS_HPP
#define FACTORY_DECODER_RS_HPP

#include <string>
#include <memory>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_RS_name;
extern const std::string Decoder_RS_prefix;
struct Decoder_RS : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int t = 5; // correction power of the RS

		// deduced parameters
		int m = 0; // Gallois field order

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_RS_prefix);
		virtual ~parameters() = default;
		Decoder_RS::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const tools::RS_polynomial_generator &GF,
		                                 const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO_HIHO<B,Q>* build_hiho(const tools::RS_polynomial_generator &GF,
		                                           const std::unique_ptr<module::Encoder<B>>& encoder = nullptr) const;

	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params, const tools::RS_polynomial_generator &GF,
	                                        const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO_HIHO<B,Q>* build_hiho(const parameters &params, const tools::RS_polynomial_generator &GF,
	                                                  const std::unique_ptr<module::Encoder<B>>& encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_RS_HPP */
