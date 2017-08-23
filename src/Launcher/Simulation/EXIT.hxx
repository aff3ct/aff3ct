#include "Simulation/EXIT/EXIT.hpp"

#include <thread>
#include <string>
#include <iostream>

#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "EXIT.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R>
EXIT<C,B,R>
::EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, params, stream)
{
}

template <class C, typename B, typename R>
EXIT<C,B,R>
::~EXIT()
{
}

template <class C, typename B, typename R>
void EXIT<C,B,R>
::build_args()
{
	Launcher::build_args();

	factory::EXIT         ::build_args(this->req_args, this->opt_args);
	factory::Source       ::build_args(this->req_args, this->opt_args);
	factory::Modem        ::build_args(this->req_args, this->opt_args);
	factory::Channel      ::build_args(this->req_args, this->opt_args);
	factory::Terminal_EXIT::build_args(this->req_args, this->opt_args);

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

template <class C, typename B, typename R>
void EXIT<C,B,R>
::store_args()
{
	Launcher::store_args();

	factory::EXIT::store_args(this->ar.get_args(), params);

	params.src.seed = params.local_seed;

	factory::Source::store_args(this->ar.get_args(), params.src);

	auto K = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params.src.K : params.cdc.K;
	auto N = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params.src.K : params.cdc.N;

	params.src.K = params.src.K == 0 ? K : params.src.K;
	params.mdm.N = N;

	factory::Modem::store_args(this->ar.get_args(), params.mdm);

	params.chn.N         = params.mdm.N_mod;
	params.chn.complex   = params.mdm.complex;
	params.chn.add_users = params.mdm.type == "SCMA";
	params.chn.seed      = params.local_seed;

	factory::Channel::store_args(this->ar.get_args(), params.chn);

	params.ter.N = N;

	factory::Terminal_EXIT::store_args(this->ar.get_args(), params.ter);

	if (params.src.type == "AZCW" || params.cdc.enc.type == "AZCW")
	{
		params.src    .type = "AZCW";
		params.cdc.enc.type = "AZCW";
	}

	params.cdc.enc.seed = params.local_seed;

	params.mdm.n_frames = params.src.n_frames;
	params.chn.n_frames = params.src.n_frames;
}

template <class C, typename B, typename R>
void EXIT<C,B,R>
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

template <class C, typename B, typename R>
void EXIT<C,B,R>
::print_header()
{
	factory::EXIT         ::make_header(this->pl_sim, params    , false);
	factory::Source       ::make_header(this->pl_src, params.src, false);
	factory::Modem        ::make_header(this->pl_mdm, params.mdm, false);
	factory::Channel      ::make_header(this->pl_chn, params.chn, false);
	factory::Terminal_EXIT::make_header(this->pl_ter, params.ter, false);

	this->pl_cde.push_back(std::make_pair("Type",                             params.cde_type ));
	this->pl_cde.push_back(std::make_pair("Info. bits (K)",    std::to_string(params.cdc.K    )));
	this->pl_cde.push_back(std::make_pair("Codeword size (N)", std::to_string(params.cdc.N    )));
	this->pl_cde.push_back(std::make_pair("Code rate",         std::to_string(params.cdc.enc.R)));

	Launcher::print_header();
}

template <class C, typename B, typename R>
simulation::Simulation* EXIT<C,B,R>
::build_simu()
{
	return factory::EXIT::build<C,B,R>(params);
}
}
}
