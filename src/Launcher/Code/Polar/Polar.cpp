#include <type_traits>
#include <mipp.h>

#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Code/Polar/Polar.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
Polar<L,B,R,Q>
::Polar(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_polar("cdc"))
{
	this->params.set_cdc(params_cdc);

	if (std::is_same<L, BFER_std<B,R,Q>>::value)
		params_cdc->enable_puncturer();
}

template <class L, typename B, typename R, typename Q>
void Polar<L,B,R,Q>
::get_description_args()
{
	params_cdc->get_description(this->args);

	auto penc = params_cdc->enc->get_prefix();
	this->args.erase({penc+"-seed", "S"});

	if (params_cdc->pct != nullptr)
	{
		auto ppct = params_cdc->pct->get_prefix();
		this->args.erase({ppct+"-fra", "F"});
	}
	else
		this->args.erase({penc+"-fra", "F"});

	L::get_description_args();
}

template <class L, typename B, typename R, typename Q>
void Polar<L,B,R,Q>
::store_args()
{
	auto dec_polar = dynamic_cast<factory::Decoder_polar*>(params_cdc->dec.get());

	params_cdc->store(this->arg_vals);

	if (dec_polar->simd_strategy == "INTER")
		this->params.n_frames = mipp::N<Q>();

	if (std::is_same<Q,int8_t>() || std::is_same<Q,int16_t>())
	{
		this->params.qnt->n_bits     = 6;
		this->params.qnt->n_decimals = 1;
	}

	L::store_args();

	auto pfbg = this->params_cdc->fbg->get_prefix();

	if (!this->arg_vals.exist({pfbg+"-gen-method"}))
		if (this->params.chn->type == "BEC")
			this->params_cdc->fbg->type = "BEC";
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Simulation/BFER_ite.hpp"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_std<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_ite<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_ite<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_ite<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_ite<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
template class aff3ct::launcher::Polar<aff3ct::launcher::BFER_ite<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
