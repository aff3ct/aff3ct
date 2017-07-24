#include "Simulation/EXIT/EXIT.hpp"

#include <thread>
#include <string>
#include <iostream>

#include "Factory/Module/Interleaver.hpp"

#include "EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <typename B, typename R>
EXIT<B,R>
::EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, stream), codec(nullptr), params(new factory::EXIT::parameters())
{
	Launcher::params = params;
}

template <typename B, typename R>
EXIT<B,R>
::~EXIT()
{
	if (codec != nullptr)
		delete codec;
}

template <typename B, typename R>
void EXIT<B,R>
::build_args()
{
	Launcher::build_args();

	factory::EXIT::build_args(this->req_args, this->opt_args);
	factory::Source         ::build_args(this->req_args, this->opt_args);
	factory::Modem          ::build_args(this->req_args, this->opt_args);
	factory::Channel        ::build_args(this->req_args, this->opt_args);
	factory::Terminal_EXIT  ::build_args(this->req_args, this->opt_args);

	if (this->req_args.find({"enc-info-bits", "K"}) != this->req_args.end() ||
	    this->req_args.find({"pct-info-bits", "K"}) != this->req_args.end())
		this->req_args.erase({"src-info-bits", "K"});
	this->opt_args.erase({"src-seed",     "S"});
	this->req_args.erase({"mdm-fra-size", "N"});
	this->opt_args.erase({"mdm-fra",      "F"});
	this->opt_args.erase({"mdm-sigma"       });
	this->req_args.erase({"chn-fra-size", "N"});
	this->opt_args.erase({"chn-fra",      "F"});
	this->opt_args.erase({"chn-sigma"        });
	this->opt_args.erase({"chn-seed",     "S"});
	this->opt_args.erase({"chn-add-users"    });
	this->opt_args.erase({"chn-complex"      });
	this->req_args.erase({"ter-cw-size", "N"});
	this->req_args.erase({"ter-sig-a"       });
	this->req_args.erase({"ter-snr"         });
}

template <typename B, typename R>
void EXIT<B,R>
::store_args()
{
	Launcher::store_args();

	factory::EXIT::store_args(this->ar.get_args(), *params);

	factory::Source::store_args(this->ar.get_args(), *params->src);

	auto K = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params->src->K : params->enc->K;
	auto N = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params->src->K : params->pct->N;

	params->src->K = params->src->K == 0 ? K : params->src->K;
	params->mdm->N = N;

	factory::Modem::store_args(this->ar.get_args(), *params->mdm);

	params->chn->N         = params->mdm->N_mod;
	params->chn->complex   = params->mdm->complex;
	params->chn->add_users = params->mdm->type == "SCMA";

	factory::Channel::store_args(this->ar.get_args(), *params->chn);

	params->ter->N = N;

	factory::Terminal_EXIT::store_args(this->ar.get_args(), *params->ter);

	if (params->src->type == "AZCW" || params->enc->type == "AZCW")
	{
		params->src->type = "AZCW";
		params->enc->type = "AZCW";
	}

	params->enc->n_frames = params->src->n_frames;
	params->pct->n_frames = params->src->n_frames;
	params->mdm->n_frames = params->src->n_frames;
	params->chn->n_frames = params->src->n_frames;
	params->dec->n_frames = params->src->n_frames;
}

template <typename B, typename R>
void EXIT<B,R>
::group_args()
{
	Launcher::group_args();

	this->arg_group.push_back({factory::Simulation ::prefix, factory::Simulation ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Source     ::prefix, factory::Source     ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Encoder    ::prefix, factory::Encoder    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Interleaver::prefix, factory::Interleaver::name + " parameter(s)"});
	this->arg_group.push_back({factory::Modem      ::prefix, factory::Modem      ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Channel    ::prefix, factory::Channel    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Decoder    ::prefix, factory::Decoder    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Monitor    ::prefix, factory::Monitor    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Terminal   ::prefix, factory::Terminal   ::name + " parameter(s)"});
}

template <typename B, typename R>
void EXIT<B,R>
::print_header()
{
	factory::EXIT::header(this->pl_sim, *params);
	factory::Source         ::header(this->pl_src, *params->src);
	factory::Modem          ::header(this->pl_mdm, *params->mdm);
	factory::Channel        ::header(this->pl_chn, *params->chn);
	factory::Terminal_EXIT  ::header(this->pl_ter, *params->ter);

	Launcher::print_header();
}

template <typename B, typename R>
simulation::Simulation* EXIT<B,R>
::build_simu()
{
	this->build_codec();
	return factory::EXIT::build<B,R>(*params, *codec);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::EXIT<B_8,R_8>;
template class aff3ct::launcher::EXIT<B_16,R_16>;
template class aff3ct::launcher::EXIT<B_32,R_32>;
template class aff3ct::launcher::EXIT<B_64,R_64>;
#else
template class aff3ct::launcher::EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation
