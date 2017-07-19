#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>
#include <type_traits>

#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "Tools/Factory/Module/Code/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/Module/Code/Turbo/Factory_flip_and_check.hpp"

#include "../Factory_decoder.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_turbo : public Factory_decoder
{
	struct parameters : Factory_decoder::parameters
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

		Factory_scaling_factor::parameters sf;
		Factory_flip_and_check::parameters fnc;
	};

	template <typename B = int, typename Q = float>
	static module::Decoder_turbo<B,Q>* build(const parameters               &params,
	                                         const module::Interleaver<int> &itl,
	                                               module::SISO<Q>          &siso_n,
	                                               module::SISO<Q>          &siso_i);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_dec, const parameters& params);

};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
