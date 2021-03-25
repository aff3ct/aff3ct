#include "Launcher/Code/Uncoded/Uncoded.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
Uncoded<L,B,R,Q>
::Uncoded(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_uncoded("cdc"))
{
	this->params.set_cdc(params_cdc);
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::get_description_args()
{
	params_cdc->get_description(this->args);

	auto penc = params_cdc->enc->get_prefix();

	this->args.erase({penc+"-info-bits", "K"});
	this->args.erase({penc+"-fra",       "F"});

	L::get_description_args();
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::store_args()
{
	L::store_args();

	params_cdc->enc->K = this->params.src->K;

	params_cdc->store(this->arg_vals);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Simulation/BFER_ite.hpp"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
