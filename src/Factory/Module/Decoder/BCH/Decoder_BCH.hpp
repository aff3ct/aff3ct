#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <cmath>

#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_BCH_name;
extern const std::string Decoder_BCH_prefix;
struct Decoder_BCH : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int t = 5; // correction power of th BCH

		// deduced parameters
		int m = 0; // Gallois field order

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_BCH_prefix);
		virtual ~parameters();
		Decoder_BCH::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const tools::BCH_polynomial_generator &GF,
		                                 module::Encoder<B> *encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_HIHO<B>* build_hiho(const tools::BCH_polynomial_generator &GF,
		                                    module::Encoder<B> *encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SIHO_HIHO<B,Q>* build_siho_hiho(const tools::BCH_polynomial_generator &GF,
		                                                module::Encoder<B> *encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params, const tools::BCH_polynomial_generator &GF,
	                                        module::Encoder<B> *encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_HIHO<B>* build_hiho(const parameters &params, const tools::BCH_polynomial_generator &GF,
	                                    module::Encoder<B> *encoder = nullptr);
	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO_HIHO<B,Q>* build_siho_hiho(const parameters &params, const tools::BCH_polynomial_generator &GF,
	                                                module::Encoder<B> *encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
