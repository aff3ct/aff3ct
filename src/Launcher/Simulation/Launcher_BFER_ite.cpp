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
	Factory_quantizer          ::build_args(this->req_args, this->opt_args);
	Factory_monitor            ::build_args(this->req_args, this->opt_args);
	Factory_terminal_BFER      ::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_ite<B,R,Q>
::store_args()
{
	Launcher::store_args();

	Factory_simulation_BFER_ite::store_args(this->ar, *params     );
	Factory_source             ::store_args(this->ar, *params->src);
	Factory_CRC                ::store_args(this->ar, *params->crc);
	Factory_interleaver        ::store_args(this->ar, *params->itl);
	Factory_modem              ::store_args(this->ar, *params->mdm);
	Factory_channel            ::store_args(this->ar, *params->chn);
	Factory_quantizer          ::store_args(this->ar, *params->qnt);
	Factory_monitor            ::store_args(this->ar, *params->mnt);
	Factory_terminal_BFER      ::store_args(this->ar, *params->ter);

	params->chn->complex   = params->mdm->complex;
	params->chn->add_users = params->mdm->type == "SCMA";

	if (!std::is_integral<Q>())
		params->qnt->type = "NO";
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
	Factory_quantizer          ::group_args(this->arg_group);
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
	Factory_quantizer          ::header(this->pl_qnt,                 *params->qnt);
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
