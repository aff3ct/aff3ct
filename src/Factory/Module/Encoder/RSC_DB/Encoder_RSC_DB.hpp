#ifndef FACTORY_ENCODER_RSC_DB_HPP
#define FACTORY_ENCODER_RSC_DB_HPP

#include <string>

#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_RSC_DB : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Encoder::parameters
	{
		virtual ~parameters() {}

		template <typename B = int>
		module::Encoder_RSC_DB<B>* build() const;

		bool        buffered = true;
		std::string standard = "DVB-RCS1";
	};

	template <typename B = int>
	static module::Encoder_RSC_DB<B>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_ENCODER_RSC_DB_HPP */
