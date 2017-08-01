#ifndef FACTORY_ENCODER_TURBO_DB_HPP
#define FACTORY_ENCODER_TURBO_DB_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"

#include "Factory/Module/Interleaver.hpp"
#include "Factory/Module/Code/RSC_DB/Encoder_RSC_DB.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_turbo_DB : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Encoder::parameters
	{
		virtual ~parameters() {}

		std::string json_path = "";

		Interleaver::parameters itl;
		Encoder_RSC_DB::parameters sub;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters                &params,
	                                 const module::Interleaver<int>  &itl,
	                                       module::Encoder_RSC_DB<B> &sub_enc);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_enc, params_list& head_itl, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_DB_HPP */
