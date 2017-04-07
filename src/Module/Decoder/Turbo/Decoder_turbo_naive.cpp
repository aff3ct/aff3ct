#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Decoder_turbo_naive.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_turbo_naive<B,R>
::Decoder_turbo_naive(const int& K,
                      const int& N_without_tb,
                      const int& n_ite,
                      const Interleaver<int> &pi,
                      SISO<R> &siso_n,
                      SISO<R> &siso_i,
                      Scaling_factor<R> &scaling_factor,
                      const bool buffered_encoding,
                      const std::string name)
: Decoder_turbo<B,R>(K, N_without_tb, n_ite, pi, siso_n, siso_i, scaling_factor, buffered_encoding, name)
{
}

template <typename B, typename R>
Decoder_turbo_naive<B,R>
::~Decoder_turbo_naive()
{
}

template <typename B, typename R>
void Decoder_turbo_naive<B,R>
::_hard_decode(const R *Y_N, B *V_K)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	const auto n_frames = this->get_simd_inter_frame_level();
	const auto tail_n_2 = this->siso_n.tail_length() / 2;
	const auto tail_i_2 = this->siso_i.tail_length() / 2;

	// iterative turbo decoding process
	for (auto ite = 1; ite <= this->n_ite; ite++)
	{
		// sys + ext
		for (auto i = 0; i < this->K * n_frames; i++) 
			this->l_sen[i] = this->l_sn[i] + this->l_e1n[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i++) 
			this->l_sen[i] = this->l_sn[i];
	
		// SISO in the natural domain
		this->siso_n.soft_decode(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), n_frames);

		// apply the scaling factor
		this->scaling_factor(this->l_e2n, 2 * (ite -1));

		// make the interleaving
		this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1, n_frames);

		// sys + ext
		for (auto i = 0; i < this->K * n_frames; i++) 
			this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i++) 
			this->l_sei[i] = this->l_si[i];

		// SISO in the interleave domain
		this->siso_i.soft_decode(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), n_frames);

		if (ite != this->n_ite)
			// apply the scaling factor
			this->scaling_factor(this->l_e2i, 2 * (ite -1) +1);
		else
			// add the systematic information to the extrinsic information, gives the a posteriori information
			for (auto i = 0; i < this->K * n_frames; i++)
				this->l_e2i[i] += this->l_sei[i];

		// make the deinterleaving
		this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1, n_frames);
	}
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	for (auto i = 0; i < this->K * n_frames; i++)
		this->s[i] = this->l_e1n[i] < 0;

	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_naive<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_naive<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_naive<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_naive<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
