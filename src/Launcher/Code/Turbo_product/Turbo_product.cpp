#include <type_traits>
// #include <mipp.h>

#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Code/Turbo_product/Turbo_product.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
Turbo_product<L,B,R,Q>
::Turbo_product(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_turbo_product("cdc"))
{
	this->params.set_cdc(params_cdc);
}

template <class L, typename B, typename R, typename Q>
void Turbo_product<L,B,R,Q>
::get_description_args()
{
	params_cdc->get_description(this->args);

	auto penc = params_cdc->enc->get_prefix();
	auto pitl = params_cdc->itl->get_prefix();

	this->args.erase({penc+"-fra",  "F"});
	this->args.erase({penc+"-seed", "S"});

	L::get_description_args();

	auto psrc = this->params.src->get_prefix();
	this->args.erase({psrc+"-info-bits", "K"});
	// this->args.erase({psrc+"-fra",       "F"});
}

template <class L, typename B, typename R, typename Q>
void Turbo_product<L,B,R,Q>
::store_args()
{
	auto enc_tur = dynamic_cast<factory::Encoder_turbo_product*>(params_cdc->enc.get());
	auto dec_tur = dynamic_cast<factory::Decoder_turbo_product*>(params_cdc->dec.get());

	params_cdc->store(this->arg_vals);

	// if (params_cdc->dec->sub->simd_strategy == "INTER")
	// 	this->params.src->n_frames = mipp::N<Q>();
	// if (params_cdc->dec->sub->simd_strategy == "INTRA")
	// 	this->params.src->n_frames = (int)std::ceil(mipp::N<Q>() / 8.f);

	if (std::is_same<Q,int8_t>())
	{
		this->params.qnt->n_bits     = 6;
		this->params.qnt->n_decimals = 2;
	}
	else if (std::is_same<Q,int16_t>())
	{
		this->params.qnt->n_bits     = 8;
		this->params.qnt->n_decimals = 3;
	}

	L::store_args();

	params_cdc->enc      ->n_frames = this->params.src->n_frames;
	params_cdc->dec      ->n_frames = this->params.src->n_frames;
	params_cdc->itl->core->n_frames = this->params.src->n_frames;
	enc_tur        ->sub ->n_frames = this->params.src->n_frames;
	dec_tur        ->sub ->n_frames = this->params.src->n_frames;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::Turbo_product<aff3ct::launcher::BFER_std<B_8 ,R_8 ,Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Turbo_product<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Turbo_product<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Turbo_product<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Turbo_product<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
