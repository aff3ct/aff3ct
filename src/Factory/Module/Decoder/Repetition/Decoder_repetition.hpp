#ifndef FACTORY_DECODER_REPETITION_HPP
#define FACTORY_DECODER_REPETITION_HPP

#include <string>

#include "Module/Decoder/Decoder_SIHO.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_repetition : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Decoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional parameters
		bool buffered = true;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Decoder_repetition::prefix);
		virtual ~parameters();
		Decoder_repetition::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Decoder_SIHO<B,Q>* build() const;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_SIHO<B,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_DECODER_REPETITION_HPP */
