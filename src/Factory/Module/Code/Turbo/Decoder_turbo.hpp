#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>
#include <type_traits>

#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "Factory/Module/Interleaver.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_turbo : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		std::string      max            = "MAX";
		std::string      simd_strategy  = "";
		std::string      standard       = "LTE";
		bool             self_corrected = false;
		bool             buffered       = true;
		bool             enable_json    = false;
		std::vector<int> poly           = {013, 015};
		int              n_ite          = 6;

		Interleaver::parameters itl;
		Scaling_factor::parameters sf;
		Flip_and_check::parameters fnc;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_turbo<B,Q>* build(const parameters               &params,
	                                         const module::Interleaver<int> &itl,
	                                               module::SISO<Q>          &siso_n,
	                                               module::SISO<Q>          &siso_i);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p = prefix);
	static void header(params_list& head_dec, params_list& head_itl, const parameters& params);

};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
