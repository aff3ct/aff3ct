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
	params.set_src(new factory::Source         ::parameters("src"));
	params.set_crc(new factory::CRC            ::parameters("crc"));
	params.set_itl(new factory::Interleaver    ::parameters("itl"));
	params.set_mdm(new factory::Modem          ::parameters("mdm"));
	params.set_chn(new factory::Channel        ::parameters("chn"));
	params.set_qnt(new factory::Quantizer      ::parameters("qnt"));
	params.set_mnt_er(new factory::Monitor_BFER::parameters("mnt"));
	params.set_ter(new factory::Terminal       ::parameters("ter"));
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::get_description_args()
{
	Launcher::get_description_args();

	params.     get_description(this->args);
	params.src->get_description(this->args);
	params.crc->get_description(this->args);
	params.itl->get_description(this->args);
	params.mdm->get_description(this->args);
	params.chn->get_description(this->args);
	if (std::is_integral<Q>())
	params.qnt->get_description(this->args);
	params.mnt_er->get_description(this->args);
	params.ter->get_description(this->args);

	auto psrc = params.src     ->get_prefix();
	auto pcrc = params.crc     ->get_prefix();
	auto pitl = params.itl     ->get_prefix();
	auto penc = params.cdc->enc->get_prefix();
	auto ppct = std::string("pct");
	auto pmdm = params.mdm     ->get_prefix();
	auto pchn = params.chn     ->get_prefix();
	auto pqnt = params.qnt     ->get_prefix();
	auto pmnt = params.mnt_er  ->get_prefix();
	auto pter = params.ter     ->get_prefix();
	auto psim = params.          get_prefix();

	if (this->args.exist({penc+"-info-bits", "K"}) || this->args.exist({ppct+"-info-bits", "K"}))
		this->args.erase({psrc+"-info-bits", "K"});

	this->args.erase({psrc+"-seed",      "S"});
	this->args.erase({pcrc+"-info-bits", "K"});
	this->args.erase({pcrc+"-fra",       "F"});
	this->args.erase({pitl+"-size"          });
	this->args.erase({pitl+"-fra",       "F"});
	this->args.erase({pitl+"-seed",      "S"});
	this->args.erase({pmdm+"-fra-size",  "N"});
	this->args.erase({pmdm+"-fra",       "F"});
	this->args.erase({pmdm+"-noise"         });
	this->args.erase({pchn+"-fra-size",  "N"});
	this->args.erase({pchn+"-fra",       "F"});
	this->args.erase({pchn+"-noise"         });
	this->args.erase({pchn+"-seed",      "S"});
	this->args.erase({pchn+"-add-users"     });
	this->args.erase({pchn+"-complex"       });
	this->args.erase({pqnt+"-size",      "N"});
	this->args.erase({pqnt+"-fra",       "F"});
	this->args.erase({pmnt+"-info-bits", "K"});
	this->args.erase({pmnt+"-fra",       "F"});
	this->args.erase({pmnt+"-max-frame", "n"});
	this->args.erase({pter+"-info-bits", "K"});
	this->args.erase({pter+"-cw-size",   "N"});
	this->args.erase({psim+"-mutinfo"       });
}

template <typename B, typename R, typename Q>
void BFER_ite<B,R,Q>
::store_args()
{
	Launcher::store_args();

	params.store(this->arg_vals);

	params.src->seed = params.local_seed;

	params.src->store(this->arg_vals);

	auto psrc = params.src->get_prefix();

	auto K    = this->args.exist({psrc+"-info-bits", "K"}) ? params.src->K : params.cdc->K;
	auto N    = this->args.exist({psrc+"-info-bits", "K"}) ? params.src->K : params.cdc->N;
	auto N_cw = this->args.exist({psrc+"-info-bits", "K"}) ? params.src->K : params.cdc->N_cw;

	params.crc->store(this->arg_vals);

	params.crc->K = K - params.crc->size;
	params.src->K = params.src->K == 0 ? params.crc->K : params.src->K;

	params.itl->core->size = N;
	params.itl->core->seed = params.local_seed;

	params.itl->store(this->arg_vals);

	params.mdm->N = N;

	params.mdm->store(this->arg_vals);

	params.chn->N         = params.mdm->N_mod;
	params.chn->complex   = params.mdm->complex;
	params.chn->add_users = params.mdm->type == "SCMA";
	params.chn->seed      = params.local_seed;

	params.chn->store(this->arg_vals);

	params.mdm->channel_type = params.chn->type;

	params.qnt->size = params.mdm->N_fil;

	if (std::is_integral<Q>())
		params.qnt->store(this->arg_vals);

	params.mnt_er->K = params.coded_monitoring ? N_cw : params.src->K;

	params.mnt_er->store(this->arg_vals);

	params.ter->store(this->arg_vals);

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

	if (params.err_track_revert)
	{
		params.src->type = "USER";
		params.src->path = params.err_track_path + std::string("_$noise.src");

		params.cdc->enc->type = "USER";
		params.cdc->enc->path = params.err_track_path + std::string("_$noise.enc");

		if (params.itl->core->uniform)
		{
			params.itl->core->type = "USER";
			params.itl->core->path = params.err_track_path + std::string("_$noise.itl");
		}

		params.chn->type = "USER";
		params.chn->path = params.err_track_path + std::string("_$noise.chn");
	}

	params.cdc->enc->seed = params.local_seed;

	params.crc->      n_frames = params.src->n_frames;
	params.itl->core->n_frames = params.src->n_frames;
	params.mdm->      n_frames = params.src->n_frames;
	params.chn->      n_frames = params.src->n_frames;
	params.qnt->      n_frames = params.src->n_frames;
	params.mnt_er->   n_frames = params.src->n_frames;

	params.mnt_er->max_frame = params.max_frame;

#ifdef AFF3CT_MPI
	auto pter = params.ter->get_prefix();
	if (!this->arg_vals.exist({pter+"-freq"}))
		params.ter->frequency = params.mpi_comm_freq;
#endif
}

template <typename B, typename R, typename Q>
simulation::Simulation* BFER_ite<B,R,Q>
::build_simu()
{
	return factory::BFER_ite::build<B,R,Q>(params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::launcher::BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::launcher::BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::launcher::BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::launcher::BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

