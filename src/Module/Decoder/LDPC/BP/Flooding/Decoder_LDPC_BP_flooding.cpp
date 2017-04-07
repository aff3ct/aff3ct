#include <chrono>
#include <limits>
#include <stdexcept>

#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

// constexpr int C_to_V_max = 15; // saturation value for the LLRs/extrinsics

template <typename B, typename R>
Decoder_LDPC_BP_flooding<B,R>
::Decoder_LDPC_BP_flooding(const int &K, const int &N, const int& n_ite,
                           const AList_reader &alist_data,
                           const bool enable_syndrome,
                           const int syndrome_depth,
                           const int n_frames,
                           const std::string name)
: Decoder_SISO<B,R>      (K, N, n_frames, 1, name                      ),
  cur_frame              (0                                            ),
  n_ite                  (n_ite                                        ),
  n_V_nodes              (N                                            ), // same as N but more explicit
  n_C_nodes              ((int)alist_data.get_n_CN()                   ),
  n_branches             ((int)alist_data.get_n_branches()             ),
  enable_syndrome        (enable_syndrome                              ),
  syndrome_depth         (syndrome_depth                               ),
  init_flag              (false                                        ),

  n_variables_per_parity (alist_data.get_n_VN_per_CN()                 ),
  n_parities_per_variable(alist_data.get_n_CN_per_VN()                 ),
  transpose              (alist_data.get_branches_transpose()          ),

  Lp_N                   (N,                                          -1), // -1 in order to fail when AZCW
  C_to_V                 (n_frames, mipp::vector<R>(this->n_branches, 0)),
  V_to_C                 (n_frames, mipp::vector<R>(this->n_branches, 0))
{
	if (n_ite <= 0)
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_flooding: \"n_ite\" has to be greater than 0.");
	if (syndrome_depth <= 0)
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_flooding: \"syndrome_depth\" has to be greater "
		                            "than 0.");
	if (N != (int)alist_data.get_n_VN())
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_flooding: \"N\" is not compatible with the alist "
		                            "file.");
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding<B,R>
::~Decoder_LDPC_BP_flooding()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::_soft_decode(const R *Y_N1, R *Y_N2)
{
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V[cur_frame].begin(), this->C_to_V[cur_frame].end(), (R)0);

		if (cur_frame == Decoder<B,R>::n_frames -1)
			this->init_flag = false;
	}

	// actual decoding
	this->BP_decode(Y_N1);

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < this->N; i++)
		Y_N2[i] = this->Lp_N[i] - Y_N1[i];

	// saturate<R>(Y_N2, (R)-C_to_V_max, (R)C_to_V_max);

	cur_frame = (cur_frame +1) % SISO<R>::n_frames;
}

template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::_hard_decode(const R *Y_N, B *V_K)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	// memory zones initialization
	if (this->init_flag)
	{
		std::fill(this->C_to_V[cur_frame].begin(), this->C_to_V[cur_frame].end(), (R)0);

		if (cur_frame == Decoder<B,R>::n_frames -1)
			this->init_flag = false;
	}
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	this->BP_decode(Y_N);
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto i = 0; i < this->K; i++)
		V_K[i] = !(this->Lp_N[i] >= 0);

	// set the flag so C_to_V structure can be reset to 0 only at the beginning of the loop in iterative decoding
	if (cur_frame == Decoder<B,R>::n_frames -1)
		this->init_flag = true;

	cur_frame = (cur_frame +1) % SISO<R>::n_frames;
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

// BP algorithm
template <typename B, typename R>
void Decoder_LDPC_BP_flooding<B,R>
::BP_decode(const R *Y_N)
{
	auto cur_syndrome_depth = 0;

	// actual decoding
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		// specific inner code depending on the selected implementation (min-sum or sum-product for example)
		auto syndrome = this->BP_process(Y_N, this->V_to_C[cur_frame], this->C_to_V[cur_frame]);
		
		// make a saturation
		// saturate<R>(this->C_to_V, (R)-C_to_V_max, (R)C_to_V_max);

		// stop criterion
		if (this->enable_syndrome && syndrome)
		{
			cur_syndrome_depth++;
			if (cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			cur_syndrome_depth = 0;
	}

	// begining of the iteration upon all the matrix lines
	R *C_to_V_ptr = this->C_to_V[cur_frame].data();
	for (auto i = 0; i < this->n_V_nodes; i++) 
	{
		const auto length = this->n_parities_per_variable[i];

		auto sum_C_to_V = (R)0;
		for (auto j = 0; j < length; j++)
			sum_C_to_V += C_to_V_ptr[j];

		// filling the output
		this->Lp_N[i] = Y_N[i] + sum_C_to_V;

		C_to_V_ptr += length;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
