#ifndef FACTORY_ENCODER_TURBO_PRODUCT_HPP
#define FACTORY_ENCODER_TURBO_PRODUCT_HPP

#include <string>
#include <type_traits>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Turbo_product/Encoder_turbo_product.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_turbo_product_name;
extern const std::string Encoder_turbo_product_prefix;
struct Encoder_turbo_product : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		int parity_extended = false;

		// depending parameters
		Encoder_BCH ::parameters *sub;
		Interleaver ::parameters *itl;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_turbo_product_prefix);
		virtual ~parameters();
		Encoder_turbo_product::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_turbo_product<B>* build(const module::Interleaver<B> &itl,
		                                                   module::Encoder_BCH<B> &enc_n,
		                                                   module::Encoder_BCH<B> &enc_i) const;
	};

	template <typename B = int>
	static module::Encoder_turbo_product<B>* build(const parameters             &params,
	                                               const module::Interleaver<B> &itl,
	                                                     module::Encoder_BCH<B> &enc_n,
	                                                     module::Encoder_BCH<B> &enc_i);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_PRODUCT_HPP */
