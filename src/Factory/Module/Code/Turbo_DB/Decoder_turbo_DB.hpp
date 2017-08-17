#ifndef FACTORY_DECODER_TURBO_DB_HPP
#define FACTORY_DECODER_TURBO_DB_HPP

#include <string>
#include <type_traits>

#include "Module/Decoder/RSC_DB/BCJR//Decoder_RSC_DB_BCJR.hpp"
#include "Module/Decoder/Turbo_DB/Decoder_turbo_DB.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "Factory/Module/Interleaver.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Module/Code/RSC_DB/Decoder_RSC_DB.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_turbo_DB : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		int n_ite = 6;

		Decoder_RSC_DB   ::parameters sub;
		Interleaver      ::parameters itl;
		Scaling_factor   ::parameters sf;
		Flip_and_check_DB::parameters fnc;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_turbo_DB<B,Q>* build(const parameters                       &params,
	                                            const module::Interleaver<int>         &itl,
	                                                  module::Decoder_RSC_DB_BCJR<B,Q> &siso_n,
	                                                  module::Decoder_RSC_DB_BCJR<B,Q> &siso_i);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, params_list& head_itl, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_DECODER_TURBO_DB_HPP */
