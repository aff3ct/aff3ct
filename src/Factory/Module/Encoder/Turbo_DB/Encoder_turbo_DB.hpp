#ifndef FACTORY_ENCODER_TURBO_DB_HPP
#define FACTORY_ENCODER_TURBO_DB_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_turbo_DB : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		std::string json_path = "";

		// depending parameters
		Interleaver   ::parameters *itl;
		Encoder_RSC_DB::parameters *sub;

		// ---------------------------------------------------------------------------------------------------- METHODS
		parameters(const std::string &p = Encoder_turbo_DB::prefix);
		virtual ~parameters();
		Encoder_turbo_DB::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder<B>* build(const module::Interleaver<B> &itl, module::Encoder_RSC_DB<B> &sub_enc) const;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters                &params,
	                                 const module::Interleaver<B>    &itl,
	                                       module::Encoder_RSC_DB<B> &sub_enc);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_DB_HPP */
