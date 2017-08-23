#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Encoder/RSC/Encoder_RSC.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace factory
{
struct Encoder_turbo : public Encoder
{
	static const std::string name;
	static const std::string prefix;

	template <class E1 = Encoder_RSC, class E2 = E1>
	struct parameters : Encoder::parameters
	{
		virtual ~parameters() {}

		template <typename B = int>
		module::Encoder<B>* build(const module::Interleaver<B> &itl,
		                                module::Encoder_sys<B> *enc_n,
		                                module::Encoder_sys<B> *enc_i = nullptr) const;

		std::string json_path = "";

		Interleaver::parameters itl;
		typename E1::parameters sub1;
		typename E2::parameters sub2;
	};

	template <typename B = int, class E1 = Encoder_RSC, class E2 = E1>
	static module::Encoder<B>* build(const parameters<E1,E2>      &params,
	                                 const module::Interleaver<B> &itl,
	                                       module::Encoder_sys<B> *enc_n,
	                                       module::Encoder_sys<B> *enc_i = nullptr);

	template <class E1 = Encoder_RSC, class E2 = E1>
	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	template <class E1 = Encoder_RSC, class E2 = E1>
	static void store_args(const arg_val_map &vals, parameters<E1,E2> &params, const std::string p = prefix);
	template <class E1 = Encoder_RSC, class E2 = E1>
	static void make_header(params_list& head_enc, params_list& head_itl, const parameters<E1,E2>& params, const bool full = true);
};
}
}

#include "Encoder_turbo.hxx"

#endif /* FACTORY_ENCODER_TURBO_HPP */
