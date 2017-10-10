#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Perf/hard_decision.h"

#include "Decoder_turbo_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo_std<B,R>
::Decoder_turbo_std(const int& K,
                    const int& N,
                    const int& n_ite,
                    const Interleaver<R> &pi,
                    Decoder_SISO<R> &siso_n,
                    Decoder_SISO<R> &siso_i,
                    const bool buffered_encoding,
                    const std::string name)
: Decoder(K, N, siso_n.get_n_frames(), siso_n.get_simd_inter_frame_level(), name),
  Decoder_turbo<B,R>(K, N, n_ite, pi, siso_n, siso_i, buffered_encoding, name)
{
}

template <typename B, typename R>
Decoder_turbo_std<B,R>
::~Decoder_turbo_std()
{
}

template <typename B, typename R>
void Decoder_turbo_std<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	const auto n_frames = this->get_simd_inter_frame_level();
	const auto tail_n_2 = this->siso_n.tail_length() / 2;
	const auto tail_i_2 = this->siso_i.tail_length() / 2;

	// iterative turbo decoding process
	bool stop = false;
	auto ite  = 1;
	do
	{
		// sys + ext
		for (auto i = 0; i < this->K * n_frames; i++)
			this->l_sen[i] = this->l_sn[i] + this->l_e1n[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i++)
			this->l_sen[i] = this->l_sn[i];
	
		// SISO in the natural domain
		this->siso_n.decode_siso(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), n_frames);

		for (auto cb : this->callbacks_siso_n)
		{
			stop = cb(ite, this->l_sen, this->l_e2n, this->s);
			if (stop) break;
		}

		if (!stop)
		{
			// make the interleaving
			this->pi.interleave(this->l_e2n.data(), this->l_e1i.data(), frame_id, n_frames, n_frames > 1);

			// sys + ext
			for (auto i = 0; i < this->K * n_frames; i++)
				this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

			for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i++)
				this->l_sei[i] = this->l_si[i];

			// SISO in the interleave domain
			this->siso_i.decode_siso(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), n_frames);

			for (auto cb : this->callbacks_siso_i)
			{
				stop = cb(ite, this->l_sei, this->l_e2i);
				if (stop) break;
			}

			if (ite == this->n_ite || stop)
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (auto i = 0; i < this->K * n_frames; i++)
					this->l_e2i[i] += this->l_sei[i];

			// make the deinterleaving
			this->pi.deinterleave(this->l_e2i.data(), this->l_e1n.data(), frame_id, n_frames, n_frames > 1);

			// compute the hard decision only if we are in the last iteration
			if (ite == this->n_ite || stop)
				tools::hard_decide(this->l_e1n.data(), this->s.data(), this->K * n_frames);
		}

		ite++; // increment the number of iteration
	}
	while ((ite <= this->n_ite) && !stop);

	for (auto cb : this->callbacks_end)
		cb(ite -1);
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->tasks["decode_siho"]->update_timer("load",   d_load);
	this->tasks["decode_siho"]->update_timer("decode", d_decod);
	this->tasks["decode_siho"]->update_timer("store",  d_store);
	this->tasks["decode_siho"]->update_timer("total",  d_load + d_decod + d_store);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_std<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_std<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_std<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
