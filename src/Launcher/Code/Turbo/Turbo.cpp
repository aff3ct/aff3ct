#include <iostream>
#include <mipp.h>

#include "Factory/Module/Codec/Turbo/Codec_turbo.hpp"

#include "Turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <class L, typename B, typename R, typename Q>
Turbo<L,B,R,Q>
::Turbo(const int argc, const char **argv, std::ostream &stream)
: L(argc, argv, stream), params_cdc(new factory::Codec_turbo::parameters("cdc"))
{
	this->params.set_cdc(params_cdc);
}

template <class L, typename B, typename R, typename Q>
Turbo<L,B,R,Q>
::~Turbo()
{
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
::build_args()
{
	params_cdc->get_description(this->req_args, this->opt_args);

	auto penc = params_cdc->enc->get_prefix();
	auto pitl = params_cdc->itl->get_prefix();

	this->opt_args.erase({penc+"-fra",  "F"});
	this->opt_args.erase({penc+"-seed", "S"});
	this->opt_args.erase({pitl+"-seed", "S"});

	L::build_args();
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
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
	params_cdc->pct      ->n_frames = this->params.src->n_frames;
	params_cdc->dec      ->n_frames = this->params.src->n_frames;
	params_cdc->itl->core->n_frames = this->params.src->n_frames;
	params_cdc->enc->sub1->n_frames = this->params.src->n_frames;
	params_cdc->enc->sub2->n_frames = this->params.src->n_frames;
	params_cdc->dec->sub1->n_frames = this->params.src->n_frames;
	params_cdc->dec->sub2->n_frames = this->params.src->n_frames;

	params_cdc->itl->core->seed = this->params.global_seed;

	if (this->params.err_track_revert)
	{
		params_cdc->enc->itl->core->type = "USER";
		params_cdc->enc->itl->core->path = this->params.err_track_path + std::string("_$snr.itl");
	}
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
::group_args()
{
	L::store_args();

	this->arg_group.push_back({params_cdc->enc->get_prefix(), params_cdc->enc->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params_cdc->pct->get_prefix(), params_cdc->pct->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params_cdc->dec->get_prefix(), params_cdc->dec->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params_cdc->itl->get_prefix(), params_cdc->itl->get_short_name() + " parameter(s)"});
}

template <class L, typename B, typename R, typename Q>
void Turbo<L,B,R,Q>
::print_header()
{
	params_cdc->get_headers(this->headers, false);

	this->titles.push_back(std::make_pair(params_cdc->enc->      get_prefix(), params_cdc->enc->     get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->enc->sub1->get_prefix(), "Sub encoder"                         ));
	this->titles.push_back(std::make_pair(params_cdc->pct->      get_prefix(), params_cdc->pct->     get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->itl->      get_prefix(), params_cdc->itl->     get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->dec->      get_prefix(), params_cdc->dec->     get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->dec->fnc ->get_prefix(), params_cdc->dec->fnc->get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->dec->sf  ->get_prefix(), params_cdc->dec->sf ->get_short_name()));
	this->titles.push_back(std::make_pair(params_cdc->dec->sub1->get_prefix(), "Sub decoder"                         ));

	L::print_header();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#include "Launcher/Simulation/BFER_std.hpp"
#ifdef MULTI_PREC
template class aff3ct::launcher::Turbo<aff3ct::launcher::BFER_std<B_8 ,R_8 ,Q_8 >,B_8 ,R_8 ,Q_8 >;
template class aff3ct::launcher::Turbo<aff3ct::launcher::BFER_std<B_16,R_16,Q_16>,B_16,R_16,Q_16>;
template class aff3ct::launcher::Turbo<aff3ct::launcher::BFER_std<B_32,R_32,Q_32>,B_32,R_32,Q_32>;
template class aff3ct::launcher::Turbo<aff3ct::launcher::BFER_std<B_64,R_64,Q_64>,B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Turbo<aff3ct::launcher::BFER_std<B,R,Q>,B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
