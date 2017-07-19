#include <thread>
#include <string>
#include <iostream>

#include "Launcher_BFER_ite.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Launcher_BFER_ite<B,R,Q>
::Launcher_BFER_ite(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, stream), codec(nullptr), params(new Factory_simulation_BFER_ite::parameters())
{
	Launcher::params = params;
}

template <typename B, typename R, typename Q>
Launcher_BFER_ite<B,R,Q>
::~Launcher_BFER_ite()
{
	if (codec != nullptr)
		delete codec;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::build_args()
{
	Launcher::build_args();

	Factory_simulation_BFER_ite::build_args(this->req_args, this->opt_args);
	Factory_source             ::build_args(this->req_args, this->opt_args);
	Factory_CRC                ::build_args(this->req_args, this->opt_args);
	Factory_interleaver        ::build_args(this->req_args, this->opt_args);
	Factory_modem              ::build_args(this->req_args, this->opt_args);
	Factory_channel            ::build_args(this->req_args, this->opt_args);
	if (std::is_integral<Q>())
		Factory_quantizer      ::build_args(this->req_args, this->opt_args);
	Factory_monitor            ::build_args(this->req_args, this->opt_args);
	Factory_terminal_BFER      ::build_args(this->req_args, this->opt_args);

	if (this->req_args.find({"enc-info-bits", "K"}) != this->req_args.end() ||
	    this->req_args.find({"pct-info-bits", "K"}) != this->req_args.end())
		this->req_args.erase({"src-info-bits", "K"});
	this->opt_args.erase({"src-seed",      "S"});
	this->req_args.erase({"crc-info-bits", "K"});
	this->opt_args.erase({"crc-fra",       "F"});
	this->req_args.erase({"itl-size"          });
	this->opt_args.erase({"itl-fra",       "F"});
	this->opt_args.erase({"itl-seed",      "S"});
	this->req_args.erase({"mod-fra-size",  "N"});
	this->opt_args.erase({"mod-fra",       "F"});
	this->opt_args.erase({"dmod-sigma"        });
	this->req_args.erase({"chn-fra-size",  "N"});
	this->opt_args.erase({"chn-fra",       "F"});
	this->opt_args.erase({"chn-sigma"         });
	this->opt_args.erase({"chn-seed",      "S"});
	this->opt_args.erase({"chn-add-users"     });
	this->opt_args.erase({"chn-complex"       });
	this->req_args.erase({"qnt-size",      "N"});
	this->opt_args.erase({"qnt-fra",       "F"});
	this->opt_args.erase({"qnt-sigma"         });
	this->req_args.erase({"mnt-size",      "K"});
	this->opt_args.erase({"mnt-fra",       "F"});
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::store_args()
{
	Launcher::store_args();

	Factory_simulation_BFER_ite::store_args(this->ar, *params);

	Factory_source::store_args(this->ar, *params->src);

	auto K = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params->src->K : params->enc->K;
	auto N = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params->src->K : params->pct->N;

	Factory_CRC::store_args(this->ar, *params->crc);

	params->crc->K = K - params->crc->size;
	params->src->K = params->src->K == 0 ? params->crc->K : params->src->K;

	params->itl->size = N;

	Factory_interleaver::store_args(this->ar, *params->itl);

	params->mdm->N = N;

	Factory_modem::store_args(this->ar, *params->mdm);

	params->chn->N         = params->mdm->N_mod;
	params->chn->complex   = params->mdm->complex;
	params->chn->add_users = params->mdm->type == "SCMA";

	Factory_channel::store_args(this->ar, *params->chn);

	params->qnt->size = params->mdm->N_fil;

	if (std::is_integral<Q>())
		Factory_quantizer::store_args(this->ar, *params->qnt);

	params->mnt->size = params->src->K;

	Factory_monitor::store_args(this->ar, *params->mnt);

	Factory_terminal_BFER::store_args(this->ar, *params->ter);

	if (!std::is_integral<Q>())
		params->qnt->type = "NO";

	if (params->coset)
		params->enc->type = "COSET";

	if (params->src->type == "AZCW" || params->enc->type == "AZCW")
	{
		params->src->type = "AZCW";
		params->enc->type = "AZCW";
	}

	params->crc->n_frames = params->src->n_frames;
	params->enc->n_frames = params->src->n_frames;
	params->itl->n_frames = params->src->n_frames;
	params->pct->n_frames = params->src->n_frames;
	params->mdm->n_frames = params->src->n_frames;
	params->chn->n_frames = params->src->n_frames;
	params->qnt->n_frames = params->src->n_frames;
	params->dec->n_frames = params->src->n_frames;
	params->mnt->n_frames = params->src->n_frames;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::group_args()
{
	Launcher::group_args();

	Factory_simulation_BFER_ite::group_args(this->arg_group);
	Factory_source             ::group_args(this->arg_group);
	Factory_CRC                ::group_args(this->arg_group);
	Factory_interleaver        ::group_args(this->arg_group);
	Factory_modem              ::group_args(this->arg_group);
	Factory_channel            ::group_args(this->arg_group);
	if (std::is_integral<Q>())
		Factory_quantizer      ::group_args(this->arg_group);
	Factory_monitor            ::group_args(this->arg_group);
	Factory_terminal_BFER      ::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::print_header()
{
	Factory_simulation_BFER_ite::header(this->pl_sim,                 *params);
	Factory_source             ::header(this->pl_src,                 *params->src);
	Factory_CRC                ::header(this->pl_crc,                 *params->crc);
	Factory_interleaver        ::header(this->pl_itl,                 *params->itl);
	Factory_modem              ::header(this->pl_mod, this->pl_demod, *params->mdm);
	Factory_channel            ::header(this->pl_chn,                 *params->chn);
	if (std::is_integral<Q>())
		Factory_quantizer      ::header(this->pl_qnt,                 *params->qnt);
	Factory_monitor            ::header(this->pl_mnt,                 *params->mnt);
	Factory_terminal_BFER      ::header(this->pl_ter,                 *params->ter);

	Launcher::print_header();
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_ite<B,R,Q>
::build_simu()
{
	this->build_codec();

#if defined(SYSTEMC)
	return new SC_Simulation_BFER_ite     <B,R,Q>(*params, *codec);
#elif defined(STARPU)
	return new SPU_Simulation_BFER_ite    <B,R,Q>(*params, *codec);
#else
	return new Simulation_BFER_ite_threads<B,R,Q>(*params, *codec);
#endif
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
