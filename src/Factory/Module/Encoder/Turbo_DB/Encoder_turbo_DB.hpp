#ifndef FACTORY_ENCODER_TURBO_DB_HPP
#define FACTORY_ENCODER_TURBO_DB_HPP

#include <vector>
#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Encoder/Turbo_DB/Encoder_turbo_DB.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_turbo_DB_name;
extern const std::string Encoder_turbo_DB_prefix;
struct Encoder_turbo_DB : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// depending parameters
		tools::auto_cloned_unique_ptr<Interleaver   ::parameters> itl;
		tools::auto_cloned_unique_ptr<Encoder_RSC_DB::parameters> sub;

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_turbo_DB_prefix);
		virtual ~parameters() = default;
		Encoder_turbo_DB::parameters* clone() const;

		virtual std::vector<std::string> get_names      () const;
		virtual std::vector<std::string> get_short_names() const;
		virtual std::vector<std::string> get_prefixes   () const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_turbo_DB<B>* build(const module::Interleaver<B> &itl, module::Encoder_RSC_DB<B> &sub_enc) const;
	};

	template <typename B = int>
	static module::Encoder_turbo_DB<B>* build(const parameters                &params,
	                                          const module::Interleaver<B>    &itl,
	                                                module::Encoder_RSC_DB<B> &sub_enc);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_DB_HPP */
