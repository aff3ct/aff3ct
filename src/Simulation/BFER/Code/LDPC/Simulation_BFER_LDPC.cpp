#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <numeric>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"
#include "Tools/Factory/LDPC/Factory_decoder_LDPC.hpp"

#include "Simulation_BFER_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_LDPC<B,R,Q>
::Simulation_BFER_LDPC(const parameters& params)
: Simulation_BFER<B,R,Q>(params),
  alist_data   (params.code.alist_path),
  info_bits_pos(this->params.code.K   )
{
	assert(this->params.code.N == (int)alist_data.get_n_VN());

	std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
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
	auto encoder = Simulation_BFER<B,R,Q>::build_encoder(tid);
	if (encoder == nullptr)
	{
		auto encoder_LDPC = Factory_encoder_LDPC<B>::build(this->params);

		if (tid == 0)
			encoder_LDPC->get_info_bits_pos(info_bits_pos);

		return encoder_LDPC;
	}
	else
		return encoder;
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_LDPC<B,R,Q>
::build_decoder(const int tid)
{
	return Factory_decoder_LDPC<B,Q>::build(this->params, alist_data, info_bits_pos);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_LDPC<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_LDPC<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_LDPC<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_LDPC<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_LDPC<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
