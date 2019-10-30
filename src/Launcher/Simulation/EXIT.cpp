#if !defined(AFF3CT_8BIT_PREC) && !defined(AFF3CT_16BIT_PREC)

#include "Factory/Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "Simulation/EXIT/EXIT.hpp"

#include "Launcher/Simulation/EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

template <typename B, typename R>
EXIT<B,R>
::EXIT(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, params, stream)
{
	params.set_src(new factory::Source      ("src"));
	params.set_mdm(new factory::Modem       ("mdm"));
	params.set_chn(new factory::Channel     ("chn"));
	params.set_qnt(new factory::Quantizer   ("qnt"));
	params.set_mnt(new factory::Monitor_EXIT("mnt"));
	params.set_ter(new factory::Terminal    ("ter"));
}

template <typename B, typename R>
void EXIT<B,R>
::get_description_args()
{
	Launcher::get_description_args();

	params.     get_description(this->args);
	params.src->get_description(this->args);
	params.mdm->get_description(this->args);
	params.chn->get_description(this->args);
	params.mnt->get_description(this->args);
	params.ter->get_description(this->args);

	auto psrc = params.src     ->get_prefix();
	auto penc = params.cdc->enc->get_prefix();
	auto pmdm = params.mdm     ->get_prefix();
	auto pchn = params.chn     ->get_prefix();
	auto pmnt = params.mnt     ->get_prefix();
	auto pter = params.ter     ->get_prefix();

	if (this->args.exist({penc+"-info-bits", "K"}))
		this->args.erase({psrc+"-info-bits", "K"});
	this->args.erase({psrc+"-seed",     "S"});
	this->args.erase({pmdm+"-fra-size", "N"});
	this->args.erase({pmdm+"-fra",      "F"});
	this->args.erase({pmdm+"-noise"       });
	this->args.erase({pchn+"-fra-size", "N"});
	this->args.erase({pchn+"-fra",      "F"});
	this->args.erase({pchn+"-noise"        });
	this->args.erase({pchn+"-seed",     "S"});
	this->args.erase({pchn+"-add-users"    });
	this->args.erase({pchn+"-complex"      });
	this->args.erase({pmnt+"-size",     "K"});
	this->args.erase({pmnt+"-fra",      "F"});
	this->args.erase({pter+"-cw-size",  "N"});
}

template <typename B, typename R>
void EXIT<B,R>
::store_args()
{
	Launcher::store_args();

	params.store(this->arg_vals);

	params.src->seed = params.local_seed;

	params.src->store(this->arg_vals);

	auto psrc = params.src->get_prefix();

	auto K = this->args.exist({psrc+"-info-bits", "K"}) ? params.src->K : params.cdc->K;
	auto N = this->args.exist({psrc+"-info-bits", "K"}) ? params.src->K : params.cdc->N;

	params.src->K = params.src->K == 0 ? K : params.src->K;
	params.mdm->N = N;

	params.mdm->store(this->arg_vals);

	params.chn->N         = params.mdm->N_mod;
	params.chn->complex   = params.mdm->complex;
	params.chn->add_users = params.mdm->type == "SCMA";
	params.chn->seed      = params.local_seed;

	params.chn->store(this->arg_vals);

	params.mdm->channel_type = params.chn->type;

	params.mnt->size = K;

	params.mnt->store(this->arg_vals);

	params.ter->store(this->arg_vals);

	if (params.src->type == "AZCW" || params.cdc->enc->type == "AZCW")
	{
		params.src     ->type = "AZCW";
		params.cdc->enc->type = "AZCW";
	}

	params.cdc->enc->seed = params.local_seed;

	params.mdm->n_frames = params.src->n_frames;
	params.chn->n_frames = params.src->n_frames;

	auto pmnt = params.mnt->get_prefix();

	if (!this->arg_vals.exist({pmnt+"-trials", "n"}) && params.cdc->K != 0)
		params.mnt->n_trials = 200000 / params.cdc->K;
}

template <typename B, typename R>
simulation::Simulation* EXIT<B,R>
::build_simu()
{
#if !defined(AFF3CT_SYSTEMC_SIMU)
	return params.build<B,R>();
#else
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC/TLM  simulation is not available.");
#endif
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::EXIT<B_32,R_32>;
template class aff3ct::launcher::EXIT<B_64,R_64>;
#else
template class aff3ct::launcher::EXIT<B,R>;
#endif
// ==================================================================================== explicit template instantiation

#endif