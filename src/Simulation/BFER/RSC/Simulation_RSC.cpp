#include <map>
#include <chrono>
#include <vector>
#include <string>
#include <cassert>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "Tools/bash_tools.h"
#include "Tools/simu_tools.h"

#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"

#include "Simulation_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
Simulation_RSC<B,R,Q,QD>
::Simulation_RSC(const t_simulation_param& simu_params,
                 const t_code_param&       code_params,
                 const t_encoder_param&    enco_params,
                 const t_mod_param&        mod_params,
                 const t_channel_param&    chan_params,
                 const t_decoder_param&    deco_params)
:  Simulation_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params)
{
	assert(code_params.N / code_params.K == 2);
}

template <typename B, typename R, typename Q, typename QD>
Simulation_RSC<B,R,Q,QD>
::~Simulation_RSC()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC<B,R,Q,QD>
::launch_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
void Simulation_RSC<B,R,Q,QD>
::snr_precompute()
{
}

template <typename B, typename R, typename Q, typename QD>
Encoder<B>* Simulation_RSC<B,R,Q,QD>
::build_encoder(const int tid)
{
	auto encoder = Factory_encoder_RSC<B>::build(this->simu_params,
	                                             this->code_params,
	                                             this->enco_params,
	                                             this->deco_params);
	if (tid == 0)
		trellis = encoder->get_trellis();
	return encoder;
}

template <typename B, typename R, typename Q, typename QD>
Decoder<B,Q>* Simulation_RSC<B,R,Q,QD>
::build_decoder(const int tid)
{
	this->barrier(tid);
	return Factory_decoder_RSC<B,Q,QD>::build(this->code_params,
	                                          this->enco_params,
	                                          this->chan_params,
	                                          this->deco_params,
	                                          trellis);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_RSC<B_8,R_8,Q_8,QD_8>;
template class Simulation_RSC<B_16,R_16,Q_16,QD_16>;
template class Simulation_RSC<B_32,R_32,Q_32,QD_32>;
template class Simulation_RSC<B_64,R_64,Q_64,QD_64>;
#else
template class Simulation_RSC<B,R,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
