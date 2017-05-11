#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Perf/Reorderer/Reorderer.hpp"

#include "Decoder_turbo_fast.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_turbo_fast<B,R>
::Decoder_turbo_fast(const int& K,
                     const int& N,
                     const int& n_ite,
                     const Interleaver<int> &pi,
                     SISO<R> &siso_n,
                     SISO<R> &siso_i,
                     Scaling_factor<R> &scaling_factor,
                     const bool buffered_encoding,
                     const std::string name)
: Decoder_turbo<B,R>(K, N, n_ite, pi, siso_n, siso_i, scaling_factor, buffered_encoding, name)
{
}

template <typename B, typename R>
Decoder_turbo_fast<B,R>
::~Decoder_turbo_fast()
{
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::_load(const R *Y_N)
{
	if (this->buffered_encoding && this->get_simd_inter_frame_level() > 1)
	{
		const auto tail_n       = this->siso_n.tail_length();
		const auto tail_i       = this->siso_i.tail_length();
		const auto frame_size   = this->N;
		const auto N_without_tb = this->N - (this->siso_n.tail_length() + this->siso_i.tail_length());
		const auto p_size       = (N_without_tb - this->K) / 2; // size of the parity

		if (this->get_simd_inter_frame_level() == mipp::nElReg<B>())
		{
			constexpr auto n_frames = mipp::nElReg<R>();

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_sn.data(), this->K);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +this->K;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pn.data(), p_size);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +this->K + p_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pi.data(), p_size);

			this->pi.interleave(this->l_sn, this->l_si, true, this->get_simd_inter_frame_level());

			// tails bit in the natural domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb + tail_n/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_sn[this->K*n_frames], tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pn[p_size*n_frames], tail_n/2);

			// tails bit in the interleaved domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb + tail_n + tail_i/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_si[this->K*n_frames], tail_i/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb + tail_n;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pi[p_size*n_frames], tail_i/2);
		}
		else
		{
			constexpr auto n_frames = mipp::nElReg<R>() / 8;

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_sn.data(), this->K);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +this->K;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pn.data(), p_size);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +this->K + p_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pi.data(), p_size);

			this->pi.interleave(this->l_sn, this->l_si, true, this->get_simd_inter_frame_level());

			// tails bit in the natural domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb + tail_n/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_sn[this->K*n_frames], tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pn[p_size*n_frames], tail_n/2);

			// tails bit in the interleaved domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb + tail_n + tail_i/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_si[this->K*n_frames], tail_i/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*frame_size +N_without_tb + tail_n;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pi[p_size*n_frames], tail_i/2);
		}

		std::fill(this->l_e1n.begin(), this->l_e1n.end(), (R)0);
	}
	else
		Decoder_turbo<B,R>::_load(Y_N);
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
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
		// l_se = sys + ext
		for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sen = mipp::Reg<R>(&this->l_sn[i]) + mipp::Reg<R>(&this->l_e1n[i]);
			r_l_sen.store(&this->l_sen[i]);
		}
		std::copy(this->l_sn .begin() +  this->K             * n_frames,
		          this->l_sn .begin() + (this->K + tail_n_2) * n_frames,
		          this->l_sen.begin() +  this->K             * n_frames);

		// SISO in the natural domain
		this->siso_n.soft_decode(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), n_frames);

		// apply the scaling factor
		this->scaling_factor(this->l_e2n, 2 * (ite -1));

		// make the interleaving
		this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1, n_frames);

		// l_se = sys + ext
		for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sei = mipp::Reg<R>(&this->l_si[i]) + mipp::Reg<R>(&this->l_e1i[i]);
			r_l_sei.store(&this->l_sei[i]);
		}
		std::copy(this->l_si .begin() +  this->K             * n_frames,
		          this->l_si .begin() + (this->K + tail_i_2) * n_frames,
		          this->l_sei.begin() +  this->K             * n_frames);

		// SISO in the interleave domain
		this->siso_i.soft_decode(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), n_frames);

		if (ite != this->n_ite)
			// apply the scaling factor
			this->scaling_factor(this->l_e2i, 2 * (ite -1) +1);
		else
			// add the systematic information to the extrinsic information, gives the a posteriori information
			for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
			{
				const auto r_post = mipp::Reg<R>(&this->l_e2i[i]) + mipp::Reg<R>(&this->l_sei[i]);
				r_post.store(&this->l_e2i[i]);
			}

		// make the deinterleaving
		this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1, n_frames);
	}
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	const auto loop_size1 = (this->K * n_frames) / mipp::nElReg<R>();
	for (auto i = 0; i < loop_size1; i++)
	{
		const auto r_post = mipp::Reg<R>(&this->l_e1n[i * mipp::nElReg<R>()]);
#if defined(MIPP_NO_INTRINSICS) && defined(_MSC_VER) 
		const auto r_dec = mipp::Reg<B>((B)r_post.sign().r) >> (sizeof(B) * 8 - 1); // s[i] = (l_e1n[i] < 0);
#else
		const auto r_dec = mipp::Reg<B>(r_post.sign().r) >> (sizeof(B) * 8 - 1); // s[i] = (l_e1n[i] < 0);
#endif
		r_dec.store(&this->s[i * mipp::nElReg<R>()]);
	}
	const auto loop_size2 = this->K * n_frames;
	for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
		this->s[i] = (this->l_e1n[i] < 0);

	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::_store(B *V_K) const
{
	if (this->get_simd_inter_frame_level() > 1)
	{
		if (this->get_simd_inter_frame_level() == mipp::nElReg<B>())
		{
			constexpr auto n_frames = mipp::nElReg<B>();

			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = V_K + f*this->K;
			Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
		}
		else
		{
			constexpr auto n_frames = mipp::nElReg<B>() / 8;

			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = V_K + f*this->K;
			Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
		}
	}
	else
		Decoder_turbo<B,R>::_store(V_K);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_fast<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_fast<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_fast<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_fast<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_fast<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
