#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>
#include <type_traits>

#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Module/Decoder/RSC/Decoder_RSC.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Decoder_turbo : public Decoder
{
	static const std::string name;
	static const std::string prefix;

	template <class D1 = Decoder_RSC, class D2 = D1>
	struct parameters : Decoder::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename Q = float>
		module::Decoder_turbo<B,Q>* build(const module::Interleaver<Q>  &itl,
		                                        module::Decoder_SISO<Q> &siso_n,
		                                        module::Decoder_SISO<Q> &siso_i) const;

		bool self_corrected = false;
		bool enable_json    = false;
		int  n_ite          = 6;

		typename D1   ::parameters sub1;
		typename D2   ::parameters sub2;
		Interleaver   ::parameters itl;
		Scaling_factor::parameters sf;
		Flip_and_check::parameters fnc;
	};

	template <typename B = int, typename Q = float, class D1 = Decoder_RSC, class D2 = D1>
	static module::Decoder_turbo<B,Q>* build(const parameters<D1,D2>       &params,
	                                         const module::Interleaver<Q>  &itl,
	                                               module::Decoder_SISO<Q> &siso_n,
	                                               module::Decoder_SISO<Q> &siso_i);

	template <class D1 = Decoder_RSC, class D2 = D1>
	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	template <class D1 = Decoder_RSC, class D2 = D1>
	static void store_args(const arg_val_map &vals, parameters<D1,D2> &params, const std::string p = prefix);
	template <class D1 = Decoder_RSC, class D2 = D1>
	static void make_header(params_list& head_dec, params_list& head_itl, const parameters<D1,D2>& params, const bool full = true);
};
}
}

#include "Decoder_turbo.hxx"

#endif /* FACTORY_DECODER_TURBO_HPP */
