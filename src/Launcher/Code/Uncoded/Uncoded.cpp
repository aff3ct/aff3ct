#include <iostream>

#include "Factory/Module/Codec/Uncoded/Codec_uncoded.hpp"

#include "Uncoded.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
Uncoded<L,B,R,Q>
::Uncoded(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_uncoded::parameters("cdc"))
{
	this->params.set_cdc(params_cdc);
}

template <class L, typename B, typename R, typename Q>
Uncoded<L,B,R,Q>
::~Uncoded()
{
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::build_args()
{
	params_cdc->get_description(this->req_args, this->opt_args);

	auto penc = params_cdc->enc->get_prefix();

	this->req_args.erase({penc+"-info-bits", "K"});
	this->opt_args.erase({penc+"-fra",       "F"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::store_args()
{
	L::store_args();

	params_cdc->enc->K = this->params.src->K;

	params_cdc->store(this->ar.get_args());

	params_cdc->enc->n_frames = this->params.src->n_frames;
	params_cdc->dec->n_frames = this->params.src->n_frames;
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::group_args()
{
	L::store_args();

	this->arg_group.push_back({params_cdc->enc->get_prefix(), params_cdc->enc->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params_cdc->dec->get_prefix(), params_cdc->dec->get_short_name() + " parameter(s)"});
}

template <class L, typename B, typename R, typename Q>
void Uncoded<L,B,R,Q>
::print_header()
{
	params_cdc->get_headers(this->headers, false);

	this->titles.push_back(std::make_pair(params_cdc->enc->get_prefix(), params_cdc->enc->get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->dec->get_prefix(), params_cdc->dec->get_short_name()));

	L::print_header();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/EXIT.hpp"
#include "Launcher/Simulation/BFER_std.hpp"
#include "Launcher/Simulation/BFER_ite.hpp"
#ifdef MULTI_PREC
template class aff3ct::launcher::Uncoded<aff3ct::launcher::EXIT    <B_32,R_32     >,B_32,R_32     >;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::EXIT    <B_64,R_64     >,B_64,R_64     >;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_8, R_8, Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
#if defined(PREC_32_BIT) || defined(PREC_64_BIT)
template class aff3ct::launcher::Uncoded<aff3ct::launcher::EXIT    <B,R  >,B,R  >;
#endif
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
template class aff3ct::launcher::Uncoded<aff3ct::launcher::BFER_ite<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
