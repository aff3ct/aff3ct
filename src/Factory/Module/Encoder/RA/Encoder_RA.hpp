#ifndef FACTORY_ENCODER_RA_HPP
#define FACTORY_ENCODER_RA_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_RA : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		Interleaver::parameters *itl;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string p = Encoder_RA::prefix);
		virtual ~parameters();
		Encoder_RA::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const;
		void store          (const tools::Argument_map_value &vals                                 );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder<B>* build(const module::Interleaver<B> &itl) const;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters& params, const module::Interleaver<B> &itl);
};
}
}

#endif /* FACTORY_ENCODER_RA_HPP */
