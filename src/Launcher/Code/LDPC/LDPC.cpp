#include <type_traits>
#include <string>
#include <mipp.h>

#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Simulation/BFER_ite.hpp"
#include "Launcher/Code/LDPC/LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
LDPC<L,B,R,Q>
::LDPC(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_LDPC("cdc"))
{
	this->params.set_cdc(params_cdc);

	if (std::is_same<L, BFER_std<B,R,Q>>::value)
		params_cdc->enable_puncturer();
}

bool enc_dvb_no_h_matrix(const void*, const void* enc_type)
{
	if (enc_type == nullptr)
		return false;

	return *(const std::string*)enc_type == "LDPC_DVBS2";
}

template <class L, typename B, typename R, typename Q>
void LDPC<L,B,R,Q>
::get_description_args()
{
	params_cdc->get_description(this->args);

	auto penc = params_cdc->enc->get_prefix();
	auto pdec = params_cdc->dec->get_prefix();

	this->args.erase({penc+"-fra",  "F"});
	this->args.erase({penc+"-seed", "S"});
	this->args.erase({pdec+"-seed"     });

	this->args.add_link({pdec+"-h-path"}, {penc+"-type"}, enc_dvb_no_h_matrix);


	L::get_description_args();
}

template <class L, typename B, typename R, typename Q>
void LDPC<L,B,R,Q>
::store_args()
{
	auto dec_ldpc = dynamic_cast<factory::Decoder_LDPC*>(params_cdc->dec.get());

	params_cdc->store(this->arg_vals);

	if (dec_ldpc->simd_strategy == "INTER")
		this->params.n_frames = mipp::N<Q>();

	if (std::is_same<Q,int8_t>() || std::is_same<Q,int16_t>())
	{
		this->params.qnt->n_bits     = 6;
		this->params.qnt->n_decimals = 2;
	}

	L::store_args();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Simulation/BFER_ite.hpp"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_std<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_ite<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_ite<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_ite<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_ite<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
template class aff3ct::launcher::LDPC<aff3ct::launcher::BFER_ite<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
