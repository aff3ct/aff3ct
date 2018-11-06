#include <iostream>

#include "Factory/Module/Codec/RS/Codec_RS.hpp"

#include "RS.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
RS<L,B,R,Q>
::RS(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_RS::parameters("cdc"))
{
	this->params.set_cdc(params_cdc);
}

template <class L, typename B, typename R, typename Q>
void RS<L,B,R,Q>
::get_description_args()
{
	params_cdc->get_description(this->args);

	auto penc = params_cdc->enc->get_prefix();
	auto pdec = params_cdc->dec->get_prefix();

	this->args.erase({penc+"-fra",  "F"});
	this->args.erase({penc+"-seed", "S"});

	L::get_description_args();
}

template <class L, typename B, typename R, typename Q>
void RS<L,B,R,Q>
::store_args()
{
	params_cdc->store(this->arg_vals);

	L::store_args();

	params_cdc->enc->n_frames = this->params.src->n_frames;
	params_cdc->dec->n_frames = this->params.src->n_frames;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::RS<aff3ct::launcher::BFER_std<B_8 ,R_8 ,Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::RS<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::RS<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::RS<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::RS<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
