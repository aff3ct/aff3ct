#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"

#include "Simulation_BFER_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_LDPC<B,R,Q>
::Simulation_BFER_LDPC(const parameters& params)
: Simulation_BFER<B,R,Q>(params), alist_data(params.code.alist_path)
{
	assert(this->params.code.N == (int)alist_data.get_n_VN());
}

template <typename B, typename R, typename Q>
Simulation_BFER_LDPC<B,R,Q>
::~Simulation_BFER_LDPC()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_LDPC<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_LDPC<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_LDPC<B,R,Q>
::build_encoder(const int tid)
{
	return Simulation_BFER<B,R,Q>::build_encoder(tid);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_LDPC<B,R,Q>
::build_decoder(const int tid)
{
	return Factory_decoder_LDPC<B,Q>::build(this->params, alist_data);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER_LDPC<B_8,R_8,Q_8>;
template class Simulation_BFER_LDPC<B_16,R_16,Q_16>;
template class Simulation_BFER_LDPC<B_32,R_32,Q_32>;
template class Simulation_BFER_LDPC<B_64,R_64,Q_64>;
#else
template class Simulation_BFER_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
