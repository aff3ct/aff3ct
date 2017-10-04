#include "Simulation/EXIT/EXIT.hpp"

#include <thread>
#include <string>
#include <iostream>

#include "Factory/Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <typename B, typename R>
EXIT<B,R>
::EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, params, stream)
{
	params.set_src(new factory::Source       ::parameters("src"));
	params.set_mdm(new factory::Modem        ::parameters("mdm"));
	params.set_chn(new factory::Channel      ::parameters("chn"));
	params.set_qnt(new factory::Quantizer    ::parameters("qnt"));
	params.set_mnt(new factory::Monitor_EXIT ::parameters("mnt"));
	params.set_ter(new factory::Terminal_EXIT::parameters("ter"));
}

template <typename B, typename R>
EXIT<B,R>
::~EXIT()
{
}

template <typename B, typename R>
void EXIT<B,R>
::build_args()
{
	Launcher::build_args();

	params.     get_description(this->req_args, this->opt_args);
	params.src->get_description(this->req_args, this->opt_args);
	params.mdm->get_description(this->req_args, this->opt_args);
	params.chn->get_description(this->req_args, this->opt_args);
	params.mnt->get_description(this->req_args, this->opt_args);
	params.ter->get_description(this->req_args, this->opt_args);

	auto psrc = params.src     ->get_prefix();
	auto penc = params.cdc->enc->get_prefix();
	auto pmdm = params.mdm     ->get_prefix();
	auto pchn = params.chn     ->get_prefix();
	auto pmnt = params.mnt     ->get_prefix();
	auto pter = params.ter     ->get_prefix();

	if (this->req_args.find({penc+"-info-bits", "K"}) != this->req_args.end())
		this->req_args.erase({psrc+"-info-bits", "K"});
	this->opt_args.erase({psrc+"-seed",     "S"});
	this->req_args.erase({pmdm+"-fra-size", "N"});
	this->opt_args.erase({pmdm+"-fra",      "F"});
	this->opt_args.erase({pmdm+"-sigma"       });
	this->req_args.erase({pchn+"-fra-size", "N"});
	this->opt_args.erase({pchn+"-fra",      "F"});
	this->opt_args.erase({pchn+"-sigma"        });
	this->opt_args.erase({pchn+"-seed",     "S"});
	this->opt_args.erase({pchn+"-add-users"    });
	this->opt_args.erase({pchn+"-complex"      });
	this->req_args.erase({pmnt+"-size",     "K"});
	this->opt_args.erase({pmnt+"-fra",      "F"});
	this->req_args.erase({pter+"-cw-size",  "N"});
}

template <typename B, typename R>
void EXIT<B,R>
::store_args()
{
	Launcher::store_args();

	params.store(this->ar.get_args());

	params.src->seed = params.local_seed;

	params.src->store(this->ar.get_args());

	auto psrc = params.src->get_prefix();

	auto K = this->req_args.find({psrc+"-info-bits", "K"}) != this->req_args.end() ? params.src->K : params.cdc->K;
	auto N = this->req_args.find({psrc+"-info-bits", "K"}) != this->req_args.end() ? params.src->K : params.cdc->N;

	params.src->K = params.src->K == 0 ? K : params.src->K;
	params.mdm->N = N;

	params.mdm->store(this->ar.get_args());

	params.chn->N         = params.mdm->N_mod;
	params.chn->complex   = params.mdm->complex;
	params.chn->add_users = params.mdm->type == "SCMA";
	params.chn->seed      = params.local_seed;

	params.chn->store(this->ar.get_args());

	params.mnt->size = K;

	params.mnt->store(this->ar.get_args());

	params.ter->store(this->ar.get_args());

	if (params.src->type == "AZCW" || params.cdc->enc->type == "AZCW")
	{
		params.src     ->type = "AZCW";
		params.cdc->enc->type = "AZCW";
	}

	params.cdc->enc->seed = params.local_seed;

	params.mdm->n_frames = params.src->n_frames;
	params.chn->n_frames = params.src->n_frames;

	auto pmnt = params.mnt->get_prefix();

	if (!this->ar.exist_arg({pmnt+"-trials", "n"}) && params.cdc->K != 0)
		params.mnt->n_trials = 200000 / params.cdc->K;
}

template <typename B, typename R>
void EXIT<B,R>
::group_args()
{
	Launcher::group_args();

	this->arg_group.push_back({params.     get_prefix(), "Simulation parameter(s)"});
	this->arg_group.push_back({params.src->get_prefix(), params.src->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.mdm->get_prefix(), params.mdm->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.chn->get_prefix(), params.chn->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.mnt->get_prefix(), params.mnt->get_short_name() + " parameter(s)"});
	this->arg_group.push_back({params.ter->get_prefix(), params.ter->get_short_name() + " parameter(s)"});
}

template <typename B, typename R>
void EXIT<B,R>
::print_header()
{
	auto cpy_titles = this->titles;
	this->titles.clear();

	auto pcde = "cde";
	this->titles.push_back(std::make_pair(params.     get_prefix(), "Simulation"                ));
	this->titles.push_back(std::make_pair(pcde,                     "Code"                      ));
	this->titles.push_back(std::make_pair(params.src->get_prefix(), params.src->get_short_name()));
	this->titles.push_back(std::make_pair(params.mdm->get_prefix(), params.mdm->get_short_name()));

	for (auto t : cpy_titles)
		if (t.first.find("enc") == 0)
			this->titles.push_back(t);

	this->titles.push_back(std::make_pair(params.chn->get_prefix(), params.chn->get_short_name()));

	for (auto t : cpy_titles)
		if (t.first.find("dec") == 0)
			this->titles.push_back(t);

	this->titles.push_back(std::make_pair(params.mnt->get_prefix(), params.mnt->get_short_name()));
	this->titles.push_back(std::make_pair(params.ter->get_prefix(), params.ter->get_short_name()));

	params.     get_headers(this->headers, false);
	params.src->get_headers(this->headers, false);
	params.mdm->get_headers(this->headers, false);
	params.chn->get_headers(this->headers, false);
	params.mnt->get_headers(this->headers, false);
	params.ter->get_headers(this->headers, false);

	this->headers[pcde].push_back(std::make_pair("Type",                             params.cde_type   ));
	this->headers[pcde].push_back(std::make_pair("Info. bits (K)",    std::to_string(params.cdc->K     )));
	this->headers[pcde].push_back(std::make_pair("Codeword size (N)", std::to_string(params.cdc->N     )));
	this->headers[pcde].push_back(std::make_pair("Code rate",         std::to_string(params.cdc->enc->R)));

	Launcher::print_header();
}

template <typename B, typename R>
simulation::Simulation* EXIT<B,R>
::build_simu()
{
#if !defined(SYSTEMC)
	return factory::EXIT::build<B,R>(params);
#else
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC/TLM  simulation is not available.");
#endif
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::EXIT<B_32,R_32>;
template class aff3ct::launcher::EXIT<B_64,R_64>;
#else
template class aff3ct::launcher::EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation

