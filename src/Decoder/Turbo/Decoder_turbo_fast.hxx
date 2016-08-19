#include "Decoder_turbo_fast.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

template <typename B, typename R>
Decoder_turbo_fast<B,R>
::Decoder_turbo_fast(const int& K,
                     const int& N_without_tb,
                     const int& n_ite,
                     const Interleaver<short> &pi,
                     SISO<R> &siso_n,
                     SISO<R> &siso_i,
                     Scaling_factor<R> &scaling_factor,
	                 const bool buffered_encoding,
	                 const std::string name)
: Decoder_turbo<B,R>(K, N_without_tb, n_ite, pi, siso_n, siso_i, scaling_factor, buffered_encoding, name)
{
}

template <typename B, typename R>
Decoder_turbo_fast<B,R>
::~Decoder_turbo_fast()
{
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::load(const mipp::vector<R>& Y_N)
{
	if (this->buffered_encoding && this->get_n_frames() > 1)
	{
		const auto tail_n       = this->siso_n.tail_length();
		const auto tail_i       = this->siso_i.tail_length();
		const auto frame_size   = this->N;
		const auto N_without_tb = this->N - (this->siso_n.tail_length() + this->siso_i.tail_length());
		const auto p_size       = (N_without_tb - this->K) / 2; // size of the parity

		if (this->get_n_frames() == mipp::nElReg<B>())
		{
			constexpr auto n_frames = mipp::nElReg<R>();

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_sn.data(), this->K);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +this->K;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pn.data(), p_size);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +this->K + p_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pi.data(), p_size);

			this->pi.interleave(this->l_sn, this->l_si, true);

			// tails bit in the natural domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_sn[this->K*n_frames], tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pn[p_size*n_frames], tail_n/2);

			// tails bit in the interleaved domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n + tail_i/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_si[this->K*n_frames], tail_i/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pi[p_size*n_frames], tail_i/2);
		}
		else
		{
			constexpr auto n_frames = mipp::nElReg<R>() / 8;

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_sn.data(), this->K);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +this->K;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pn.data(), p_size);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +this->K + p_size;
			Reorderer_static<R,n_frames>::apply(frames, this->l_pi.data(), p_size);

			this->pi.interleave(this->l_sn, this->l_si, true);

			// tails bit in the natural domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_sn[this->K*n_frames], tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pn[p_size*n_frames], tail_n/2);

			// tails bit in the interleaved domain
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n + tail_i/2;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_si[this->K*n_frames], tail_i/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n;
			Reorderer_static<R,n_frames>::apply(frames, &this->l_pi[p_size*n_frames], tail_i/2);
		}

		std::fill(this->l_e1n.begin(), this->l_e1n.end(), (R)0);
	}
	else
		Decoder_turbo<B,R>::load(Y_N);
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::decode()
{
	const auto n_frames = this->get_n_frames();
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
		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sen = mipp::Reg<R>(&this->l_sn[i]);
			r_l_sen.store(&this->l_sen[i]);
		}

		// SISO in the natural domain
		this->siso_n.decode(this->l_sen, this->l_pn, this->l_e2n);

		// apply the scaling factor
		this->scaling_factor(this->l_e2n, 2 * (ite -1));

		// make the interleaving
		this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1);

		// l_se = sys + ext
		for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sei = mipp::Reg<R>(&this->l_si[i]) + mipp::Reg<R>(&this->l_e1i[i]);
			r_l_sei.store(&this->l_sei[i]);
		}
		for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sei = mipp::Reg<R>(&this->l_si[i]);
			r_l_sei.store(&this->l_sei[i]);
		}

		// SISO in the interleave domain
		this->siso_i.decode(this->l_sei, this->l_pi, this->l_e2i);

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
		this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1);
	}

	// take the hard decision
	const auto loop_size1 = (this->K * n_frames) / mipp::nElReg<R>();
	for (auto i = 0; i < loop_size1; i++)
	{
		const auto r_post = mipp::Reg<R>(&this->l_e1n[i * mipp::nElReg<R>()]);
		const auto r_dec  = mipp::Reg<B>(r_post.sign().r) >> (sizeof(B) * 8 -1); // s[i] = (l_e1n[i] < 0);
		r_dec.store(&this->s[i * mipp::nElReg<R>()]);
	}
	const auto loop_size2 = this->K * n_frames;
	for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
		this->s[i] = (this->l_e1n[i] < 0);
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::store(mipp::vector<B>& V_K) const
{
	if (this->get_n_frames() > 1)
	{
		if (this->get_n_frames() == mipp::nElReg<B>())
		{
			constexpr auto n_frames = mipp::nElReg<B>();

			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = V_K.data() + f*this->K;
			Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
		}
		else
		{
			constexpr auto n_frames = mipp::nElReg<B>() / 8;

			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = V_K.data() + f*this->K;
			Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
		}
	}
	else
		Decoder_turbo<B,R>::store(V_K);
}