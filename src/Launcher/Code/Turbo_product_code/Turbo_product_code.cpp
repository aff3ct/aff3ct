#include <iostream>
#include <typeinfo>
#include <mipp.h>

#include "Launcher/Simulation/BFER_std.hpp"

#include "Turbo_product_code.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
Turbo_product_code<L,B,R,Q>
::Turbo_product_code(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_turbo_product_code::parameters("cdc"))
{
	this->params.set_cdc(params_cdc);

	if (typeid(L) == typeid(BFER_std<B,R,Q>))
		params_cdc->enable_puncturer();
}

template <class L, typename B, typename R, typename Q>
Turbo_product_code<L,B,R,Q>
::~Turbo_product_code()
{
}

template <class L, typename B, typename R, typename Q>
void Turbo_product_code<L,B,R,Q>
::get_description_args()
{
	params_cdc->get_description(this->req_args, this->opt_args);

	auto penc = params_cdc->enc->get_prefix();
	auto pitl = params_cdc->itl->get_prefix();

	this->opt_args.erase({penc+"-fra",  "F"});
	this->opt_args.erase({penc+"-seed", "S"});
	this->opt_args.erase({pitl+"-seed", "S"});

	L::get_description_args();
}

template <class L, typename B, typename R, typename Q>
void Turbo_product_code<L,B,R,Q>
::store_args()
{
	params_cdc->store(this->ar.get_args());

	if (params_cdc->dec->sub1->simd_strategy == "INTER")
		this->params.src->n_frames = mipp::N<Q>();
	if (params_cdc->dec->sub1->simd_strategy == "INTRA")
		this->params.src->n_frames = (int)std::ceil(mipp::N<Q>() / 8.f);

	if (std::is_same<Q,int8_t>())
	{
		this->params.qnt->n_bits     = 6;
		this->params.qnt->n_decimals = 2;
	}
	else if (std::is_same<Q,int16_t>())
	{
		this->params.qnt->n_bits     = 6;
		this->params.qnt->n_decimals = 3;
	}

	L::store_args();

	params_cdc->enc      ->n_frames = this->params.src->n_frames;
	if (params_cdc->pct)
	params_cdc->pct      ->n_frames = this->params.src->n_frames;
	params_cdc->dec      ->n_frames = this->params.src->n_frames;
	params_cdc->itl->core->n_frames = this->params.src->n_frames;
	params_cdc->enc->sub1->n_frames = this->params.src->n_frames;
	params_cdc->enc->sub2->n_frames = this->params.src->n_frames;
	params_cdc->dec->sub1->n_frames = this->params.src->n_frames;
	params_cdc->dec->sub2->n_frames = this->params.src->n_frames;

	params_cdc->itl->core->seed = this->params.global_seed;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#ifdef MULTI_PREC
template class aff3ct::launcher::Turbo_product_code<aff3ct::launcher::BFER_std<B_8 ,R_8 ,Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Turbo_product_code<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Turbo_product_code<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Turbo_product_code<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Turbo_product_code<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
