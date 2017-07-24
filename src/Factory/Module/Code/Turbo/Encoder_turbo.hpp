#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"

#include "Factory/Module/Interleaver.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_turbo : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Encoder::parameters
	{
		virtual ~parameters() {}

		std::string      json_path   = "";
		std::string      standard    = "LTE";
		std::vector<int> poly        = {013, 015};
		bool             buffered    = true;

		Interleaver::parameters itl;
	};

	template <typename B = int>
	static module::Encoder<B>* build(const parameters               &params,
	                                 const module::Interleaver<int> &itl,
	                                       module::Encoder_sys<B>   *enc_n,
	                                       module::Encoder_sys<B>   *enc_i = nullptr);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void header(params_list& head_enc, params_list& head_itl, const parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_HPP */
