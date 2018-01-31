#ifndef FACTORY_DECODER_TURBO_PRODUCT_CODE_HPP
#define FACTORY_DECODER_TURBO_PRODUCT_CODE_HPP

#include <string>
#include <type_traits>

#include "Module/Decoder/Decoder_HIHO.hpp"
#include "Module/Decoder/Turbo_product_code/Decoder_turbo_product_code.hpp"
#include "Module/Interleaver/Interleaver.hpp"
// #include "Module/Encoder/Turbo_product_code/Encoder_turbo_product_code.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Decoder/BCH/Decoder_BCH.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Decoder_turbo_product_code_name;
extern const std::string Decoder_turbo_product_code_prefix;
struct Decoder_turbo_product_code : public Decoder
{
	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int   n_ite = 4;
		float alpha = 0.5;
		int   n_least_reliable_positions = 2;
		int   n_competitors = 0;
		int   parity_extended = false;

		// depending parameters
		Decoder_BCH ::parameters *sub;
		Interleaver ::parameters *itl;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Decoder_turbo_product_code_prefix);
		virtual ~parameters();
		Decoder_turbo_product_code::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const module::Interleaver <Q> &itl,
		                                       module::Decoder_HIHO<B> &hiho_r,
		                                       module::Decoder_HIHO<B> &hiho_c,
		                                       module::Encoder     <B> &enc_r,
		                                       module::Encoder     <B> &enc_c,
		                                       module::Encoder     <B> *encoder = nullptr) const;

		template <typename B = int, typename Q = float>
		module::Decoder_SISO_SIHO<B,Q>* build_siso(const module::Interleaver <Q> &itl,
		                                                 module::Decoder_HIHO<B> &hiho_r,
		                                                 module::Decoder_HIHO<B> &hiho_c,
		                                                 module::Encoder     <B> &enc_r,
		                                                 module::Encoder     <B> &enc_c) const;

	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters              &params,
	                                        const module::Interleaver <Q> &itl,
	                                              module::Decoder_HIHO<B> &hiho_r,
	                                              module::Decoder_HIHO<B> &hiho_c,
	                                              module::Encoder     <B> &enc_r,
	                                              module::Encoder     <B> &enc_c,
	                                              module::Encoder     <B> *encoder = nullptr);

	template <typename B = int, typename Q = float>
	static module::Decoder_SISO_SIHO<B,Q>* build_siso(const parameters              &params,
	                                                  const module::Interleaver <Q> &itl,
	                                                        module::Decoder_HIHO<B> &hiho_r,
	                                                        module::Decoder_HIHO<B> &hiho_c,
	                                                        module::Encoder     <B> &enc_r,
	                                                        module::Encoder     <B> &enc_c);
};
}
}

#endif /* FACTORY_DECODER_TURBO_PRODUCT_CODE_HPP */
