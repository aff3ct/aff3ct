#include "Simulation/BFER/Standard/Simulation_BFER_std.hpp"
#include "Factory/Simulation/BFER/BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_std_name   = "Simulation BFER standard";
const std::string aff3ct::factory::BFER_std_prefix = "sim";

BFER_std
::BFER_std(const std::string &prefix)
: BFER(BFER_std_name, prefix)
{
}

BFER_std* BFER_std
::clone() const
{
	return new BFER_std(*this);
}

void BFER_std
::get_description(cli::Argument_map_info &args) const
{
	BFER::get_description(args);
}

void BFER_std
::store(const cli::Argument_map_value &vals)
{
	BFER::store(vals);
}

void BFER_std
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	BFER::get_headers(headers, full);
}

const Codec_SIHO* BFER_std
::get_cdc() const
{
	return dynamic_cast<Codec_SIHO*>(this->cdc.get());
}

template <typename B, typename R, typename Q>
simulation::Simulation* BFER_std
::build() const
{
	return new simulation::Simulation_BFER_std<B,R,Q>(*this);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_std::build<B_8 ,R_8 ,Q_8 >() const;
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_std::build<B_16,R_16,Q_16>() const;
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_std::build<B_32,R_32,Q_32>() const;
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_std::build<B_64,R_64,Q_64>() const;
#else
template aff3ct::simulation::Simulation* aff3ct::factory::BFER_std::build<B,R,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
