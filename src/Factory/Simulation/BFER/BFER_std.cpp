#include "Simulation/BFER/Standard/SystemC/SC_BFER_std.hpp"
#include "Simulation/BFER/Standard/Threads/BFER_std_threads.hpp"

#include "BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_std::name   = "Simulation BFER standard";
const std::string aff3ct::factory::BFER_std::prefix = "sim";

BFER_std::parameters
::parameters(const std::string prefix)
: BFER::parameters(BFER_std::name, prefix)
{
}

BFER_std::parameters
::~parameters()
{
}

BFER_std::parameters* BFER_std::parameters
::clone() const
{
	auto clone = new BFER_std::parameters(*this);

	if (src != nullptr) { clone->src = src->clone(); }
	if (crc != nullptr) { clone->crc = crc->clone(); }
	if (cdc != nullptr) { clone->cdc = dynamic_cast<Codec_SIHO::parameters*>(cdc->clone()); }
	if (mdm != nullptr) { clone->mdm = mdm->clone(); }
	if (chn != nullptr) { clone->chn = chn->clone(); }
	if (qnt != nullptr) { clone->qnt = qnt->clone(); }
	if (mnt != nullptr) { clone->mnt = mnt->clone(); }
	if (ter != nullptr) { clone->ter = ter->clone(); }

	clone->set_cdc(clone->cdc);

	return clone;
}

void BFER_std::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	BFER::parameters::get_description(req_args, opt_args);
}

void BFER_std::parameters
::store(const arg_val_map &vals)
{
	BFER::parameters::store(vals);
}

void BFER_std::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	BFER::parameters::get_headers(headers, full);
}

template <typename B, typename R, typename Q>
simulation::BFER_std<B,R,Q>* BFER_std::parameters
::build() const
{
#if defined(SYSTEMC)
	return new simulation::SC_BFER_std<B,R,Q>(*this);
#else
	return new simulation::BFER_std_threads<B,R,Q>(*this);
#endif
}

template <typename B, typename R, typename Q>
simulation::BFER_std<B,R,Q>* BFER_std
::build(const parameters &params)
{
	return params.template build<B,R,Q>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::BFER_std<B_8 ,R_8 ,Q_8 >* aff3ct::factory::BFER_std::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::BFER_std::parameters&);
template aff3ct::simulation::BFER_std<B_16,R_16,Q_16>* aff3ct::factory::BFER_std::build<B_16,R_16,Q_16>(const aff3ct::factory::BFER_std::parameters&);
template aff3ct::simulation::BFER_std<B_32,R_32,Q_32>* aff3ct::factory::BFER_std::build<B_32,R_32,Q_32>(const aff3ct::factory::BFER_std::parameters&);
template aff3ct::simulation::BFER_std<B_64,R_64,Q_64>* aff3ct::factory::BFER_std::build<B_64,R_64,Q_64>(const aff3ct::factory::BFER_std::parameters&);
#else
template aff3ct::simulation::BFER_std<B,R,Q>* aff3ct::factory::BFER_std::build<B,R,Q>(const aff3ct::factory::BFER_std::parameters&);
#endif
// ==================================================================================== explicit template instantiation
