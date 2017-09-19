#include "Simulation/BFER/Iterative/BFER_ite.hpp"

#include <thread>
#include <string>
#include <iostream>

#include "Factory/Module/Monitor/BFER/Monitor_BFER.hpp"

#include "BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
BFER_ite<B,R,Q>
::BFER_ite(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, params, stream)
{
	params.set_src(new factory::Source       ::parameters("src"));
	params.set_crc(new factory::CRC          ::parameters("crc"));
	params.set_itl(new factory::Interleaver  ::parameters("itl"));
	params.set_mdm(new factory::Modem        ::parameters("mdm"));
	params.set_chn(new factory::Channel      ::parameters("chn"));
	params.set_qnt(new factory::Quantizer    ::parameters("qnt"));
	params.set_mnt(new factory::Monitor_BFER ::parameters("mnt"));
	params.set_ter(new factory::Terminal_BFER::parameters("ter"));
}

template <typename B, typename R, typename Q>
BFER_ite<B,R,Q>
::~BFER_ite()
{
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::build_args()
{
	Launcher::build_args();

	params.     get_description(this->req_args, this->opt_args);
	params.src->get_description(this->req_args, this->opt_args);
	params.crc->get_description(this->req_args, this->opt_args);
	params.itl->get_description(this->req_args, this->opt_args);
	params.mdm->get_description(this->req_args, this->opt_args);
	params.chn->get_description(this->req_args, this->opt_args);
	if (std::is_integral<Q>())
	params.qnt->get_description(this->req_args, this->opt_args);
	params.mnt->get_description(this->req_args, this->opt_args);
	params.ter->get_description(this->req_args, this->opt_args);

	auto psrc = params.src     ->get_prefix();
	auto pcrc = params.crc     ->get_prefix();
	auto pitl = params.itl     ->get_prefix();
	auto penc = params.cdc->enc->get_prefix();
	auto ppct = std::string("pct");
	auto pmdm = params.mdm     ->get_prefix();
	auto pchn = params.chn     ->get_prefix();
	auto pqnt = params.qnt     ->get_prefix();
	auto pmnt = params.mnt     ->get_prefix();
	auto pter = params.ter     ->get_prefix();

	if (this->req_args.find({penc+"-info-bits", "K"}) != this->req_args.end() ||
	    this->req_args.find({ppct+"-info-bits", "K"}) != this->req_args.end())
		this->req_args.erase({psrc+"-info-bits", "K"});
	this->opt_args.erase({psrc+"-seed",      "S"});
	this->req_args.erase({pcrc+"-info-bits", "K"});
	this->opt_args.erase({pcrc+"-fra",       "F"});
	this->req_args.erase({pitl+"-size"          });
	this->opt_args.erase({pitl+"-fra",       "F"});
	this->opt_args.erase({pitl+"-seed",      "S"});
	this->req_args.erase({pmdm+"-fra-size",  "N"});
	this->opt_args.erase({pmdm+"-fra",       "F"});
	this->opt_args.erase({pmdm+"-sigma"        });
	this->req_args.erase({pchn+"-fra-size",  "N"});
	this->opt_args.erase({pchn+"-fra",       "F"});
	this->opt_args.erase({pchn+"-sigma"         });
	this->opt_args.erase({pchn+"-seed",      "S"});
	this->opt_args.erase({pchn+"-add-users"     });
	this->opt_args.erase({pchn+"-complex"       });
	this->req_args.erase({pqnt+"-size",      "N"});
	this->opt_args.erase({pqnt+"-fra",       "F"});
	this->opt_args.erase({pqnt+"-sigma"         });
	this->req_args.erase({pmnt+"-size",      "K"});
	this->opt_args.erase({pmnt+"-fra",       "F"});
	this->req_args.erase({pter+"-info-bits","K"});
	this->opt_args.erase({pter+"-cw-size",  "N"});
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::store_args()
{
	Launcher::store_args();

	params.store(this->ar.get_args());

	params.src->seed = params.local_seed;

	params.src->store(this->ar.get_args());

	auto psrc = params.src->get_prefix();

	auto K    = this->req_args.find({psrc+"-info-bits", "K"}) != this->req_args.end() ? params.src->K : params.cdc->K;
	auto N    = this->req_args.find({psrc+"-info-bits", "K"}) != this->req_args.end() ? params.src->K : params.cdc->N;
	auto N_cw = this->req_args.find({psrc+"-info-bits", "K"}) != this->req_args.end() ? params.src->K : params.cdc->N_cw;

	params.crc->store(this->ar.get_args());

	params.crc->K = K - params.crc->size;
	params.src->K = params.src->K == 0 ? params.crc->K : params.src->K;

	params.itl->core->size = N;
	params.itl->core->seed = params.local_seed;

	params.itl->store(this->ar.get_args());

	params.mdm->N = N;

	params.mdm->store(this->ar.get_args());

	params.chn->N         = params.mdm->N_mod;
	params.chn->complex   = params.mdm->complex;
	params.chn->add_users = params.mdm->type == "SCMA";
	params.chn->seed      = params.local_seed;

	params.chn->store(this->ar.get_args());

	params.qnt->size = params.mdm->N_fil;

	if (std::is_integral<Q>())
		params.qnt->store(this->ar.get_args());

	params.mnt->size = params.coded_monitoring ? N_cw : params.src->K;

	params.mnt->store(this->ar.get_args());

	params.ter->store(this->ar.get_args());

	if (!std::is_integral<Q>())
		params.qnt->type = "NO";

	if (params.coset)
		params.cdc->enc->type = "COSET";
	else if (params.cdc->enc->type == "COSET")
		params.coset = true;

	if (params.src->type == "AZCW" || params.cdc->enc->type == "AZCW")
	{
		params.src     ->type = "AZCW";
		params.cdc->enc->type = "AZCW";
	}

	params.cdc->enc->seed = params.local_seed;

	params.crc->      n_frames = params.src->n_frames;
	params.itl->core->n_frames = params.src->n_frames;
	params.mdm->      n_frames = params.src->n_frames;
	params.chn->      n_frames = params.src->n_frames;
	params.qnt->      n_frames = params.src->n_frames;
	params.mnt->      n_frames = params.src->n_frames;
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::group_args()
{
	Launcher::group_args();

	this->arg_group.push_back({params.     get_prefix(), "Simulation parameter(s)"});
	this->arg_group.push_back({params.src->get_prefix(), params.src->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.crc->get_prefix(), params.crc->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.itl->get_prefix(), params.itl->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.mdm->get_prefix(), params.mdm->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.chn->get_prefix(), params.chn->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.qnt->get_prefix(), params.qnt->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.mnt->get_prefix(), params.mnt->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.ter->get_prefix(), params.ter->get_short_name() + " parameter(s)"});
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::print_header()
{
	auto pcde = "cde";
	this->titles.push_back(std::make_pair(params.     get_prefix(), "Simulation"                ));
	this->titles.push_back(std::make_pair(pcde,                     "Code"                      ));
	this->titles.push_back(std::make_pair(params.src->get_prefix(), params.src->get_short_name()));
	this->titles.push_back(std::make_pair(params.crc->get_prefix(), params.crc->get_short_name()));
	this->titles.push_back(std::make_pair(params.itl->get_prefix(), params.itl->get_short_name()));
	this->titles.push_back(std::make_pair(params.mdm->get_prefix(), params.mdm->get_short_name()));
	this->titles.push_back(std::make_pair(params.chn->get_prefix(), params.chn->get_short_name()));
	if (std::is_integral<Q>())
	this->titles.push_back(std::make_pair(params.qnt->get_prefix(), params.qnt->get_short_name()));
	this->titles.push_back(std::make_pair(params.mnt->get_prefix(), params.mnt->get_short_name()));
	this->titles.push_back(std::make_pair(params.ter->get_prefix(), params.ter->get_short_name()));

	params.     get_headers(this->headers, false);
	params.src->get_headers(this->headers, false);
	params.crc->get_headers(this->headers, false);
	params.itl->get_headers(this->headers, false);
	params.mdm->get_headers(this->headers, false);
	params.chn->get_headers(this->headers, false);
	if (std::is_integral<Q>())
	params.qnt->get_headers(this->headers, false);
	params.mnt->get_headers(this->headers, false);
	params.ter->get_headers(this->headers, false);

	const auto code_rate = (float)params.cdc->K / (float)params.cdc->N;

	this->headers[pcde].push_back(std::make_pair("Type",                                params.cde_type ));
	this->headers[pcde].push_back(std::make_pair("Info. bits (K)",       std::to_string(params.cdc->K   )));
	this->headers[pcde].push_back(std::make_pair("Codeword size (N_cw)", std::to_string(params.cdc->N_cw)));
	this->headers[pcde].push_back(std::make_pair("Frame size (N)",       std::to_string(params.cdc->N   )));
	this->headers[pcde].push_back(std::make_pair("Code rate",            std::to_string(code_rate       )));

	Launcher::print_header();
}

template <typename B, typename R, typename Q>
simulation::Simulation* BFER_ite<B,R,Q>
::build_simu()
{
	return factory::BFER_ite::build<B,R,Q>(params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::launcher::BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::launcher::BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::launcher::BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

