#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <cmath>

#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
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
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build(const tools::BCH_polynomial_generator &GF, 
		                                 module::Encoder<B> *encoder = nullptr) const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params, const tools::BCH_polynomial_generator &GF, 
	                                        module::Encoder<B> *encoder = nullptr);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
