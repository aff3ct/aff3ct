#ifndef FACTORY_ENCODER_RSC_DB_HPP
#define FACTORY_ENCODER_RSC_DB_HPP

#include <string>
#include <map>

#include "Tools/Arguments/Argument_tools.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_RSC_DB_name;
extern const std::string Encoder_RSC_DB_prefix;
struct Encoder_RSC_DB : public Encoder
{
	class parameters : public Encoder::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// optional
		bool        buffered = true;
		std::string standard = "DVB-RCS1";

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Encoder_RSC_DB_prefix);
		virtual ~parameters() = default;
		Encoder_RSC_DB::parameters* clone() const;

		// parameters construction
		void get_description(tools::Argument_map_info &args) const;
		void store          (const tools::Argument_map_value &vals);
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int>
		module::Encoder_RSC_DB<B>* build() const;
	};

	template <typename B = int>
	static module::Encoder_RSC_DB<B>* build(const parameters &params);
};
}
}

#endif /* FACTORY_ENCODER_RSC_DB_HPP */
