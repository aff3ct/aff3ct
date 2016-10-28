#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Display/bash_tools.h"
#include "Tools/Perf/Reorderer/Reorderer.hpp"

#include "Decoder_turbo.hpp"

template <typename B, typename R>
Decoder_turbo<B,R>
::Decoder_turbo(const int& K,
                const int& N_without_tb,
                const int& n_ite,
                const Interleaver<short> &pi,
                SISO<R> &siso_n,
                SISO<R> &siso_i,
                Scaling_factor<R> &scaling_factor,
                const bool buffered_encoding,
                const std::string name)
: Decoder<B,R>(K, N_without_tb + siso_n.tail_length() + siso_i.tail_length(), siso_n.get_n_frames(), siso_n.get_simd_inter_frame_level(), name),
  n_ite(n_ite),
  buffered_encoding(buffered_encoding),
  pi(pi),
  siso_n(siso_n),
  siso_i(siso_i),
  scaling_factor(scaling_factor),
  l_sn ((K                  + (siso_n.tail_length() / 2)) * siso_n.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_si ((K                  + (siso_i.tail_length() / 2)) * siso_i.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_sen((K                  + (siso_n.tail_length() / 2)) * siso_n.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_sei((K                  + (siso_i.tail_length() / 2)) * siso_i.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_pn (((N_without_tb-K)/2 + (siso_n.tail_length() / 2)) * siso_n.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_pi (((N_without_tb-K)/2 + (siso_i.tail_length() / 2)) * siso_i.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_e1n((K                                              ) * siso_n.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_e2n((K                                              ) * siso_n.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_e1i((K                                              ) * siso_i.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  l_e2i((K                                              ) * siso_i.get_simd_inter_frame_level() + mipp::nElReg<R>()),
  s    ((K                                              ) * siso_n.get_simd_inter_frame_level()                    )
{
	assert(siso_n.get_n_frames()               == siso_i.get_n_frames()              );
	assert(siso_n.get_simd_inter_frame_level() == siso_i.get_simd_inter_frame_level());
}

template <typename B, typename R>
Decoder_turbo<B,R>
::~Decoder_turbo()
{
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::load(const mipp::vector<R>& Y_N)
{
	if (buffered_encoding)
		this->buffered_load(Y_N);
	else
		this->standard_load(Y_N);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::buffered_load(const mipp::vector<R>& Y_N)
{
	const auto tail_n = siso_n.tail_length();
	const auto tail_i = siso_i.tail_length();
	
	const auto N_without_tb = this->N - (siso_n.tail_length() + siso_i.tail_length());

	const auto p_size = (N_without_tb - this->K) / 2; // size of the parity
	if (this->get_simd_inter_frame_level() == 1)
	{
		std::copy(Y_N.begin()                     , Y_N.begin() + this->K           , l_sn.begin());
		std::copy(Y_N.begin() + this->K           , Y_N.begin() + this->K + 1*p_size, l_pn.begin());
		std::copy(Y_N.begin() + this->K + 1*p_size, Y_N.begin() + this->K + 2*p_size, l_pi.begin());
		pi.interleave(l_sn, l_si, false, this->get_simd_inter_frame_level());

		// tails bit in the natural domain
		std::copy(Y_N.begin() + N_without_tb           , Y_N.begin() + N_without_tb + tail_n/2, l_pn.begin() +p_size);
		std::copy(Y_N.begin() + N_without_tb + tail_n/2, Y_N.begin() + N_without_tb + tail_n  , l_sn.begin() +p_size);

		// tails bit in the interleaved domain
		std::copy(Y_N.begin() + N_without_tb + tail_n           , Y_N.begin() + N_without_tb + tail_n + tail_i/2, l_pi.begin() +p_size);
		std::copy(Y_N.begin() + N_without_tb + tail_n + tail_i/2, Y_N.begin() + N_without_tb + tail_n + tail_i  , l_si.begin() +p_size);
	}
	else
	{
		const auto n_frames = this->get_simd_inter_frame_level();
		const auto frame_size = this->N;

		std::vector<const R*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size;
		Reorderer<R>::apply(frames, l_sn.data(), this->K);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +this->K;
		Reorderer<R>::apply(frames, l_pn.data(), p_size);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +this->K + p_size;
		Reorderer<R>::apply(frames, l_pi.data(), p_size);

		pi.interleave(l_sn, l_si, true, this->get_simd_inter_frame_level());

		// tails bit in the natural domain
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n/2;
		Reorderer<R>::apply(frames, &l_sn[this->K*n_frames], tail_n/2);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N_without_tb;
		Reorderer<R>::apply(frames, &l_pn[p_size*n_frames], tail_n/2);

		// tails bit in the interleaved domain
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n + tail_i/2;
		Reorderer<R>::apply(frames, &l_si[this->K*n_frames], tail_i/2);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N_without_tb + tail_n;
		Reorderer<R>::apply(frames, &l_pi[p_size*n_frames], tail_i/2);
	}
	std::fill(l_e1n.begin(), l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::standard_load(const mipp::vector<R>& Y_N)
{
	assert(this->K == (this->N - (siso_n.tail_length() + siso_i.tail_length()) - this->K) / 2);

	const auto tail_n = siso_n.tail_length();
	const auto tail_i = siso_i.tail_length();

	if (this->get_simd_inter_frame_level() == 1)
	{
		for (auto i = 0; i < this->K; i++)
		{
			l_sn[i] = Y_N[i*3 +0];
			l_pn[i] = Y_N[i*3 +1];
			l_pi[i] = Y_N[i*3 +2];
		}
		pi.interleave(l_sn, l_si, false, this->get_simd_inter_frame_level());

		// tails bit in the natural domain
		for (auto i = 0; i < tail_n/2; i++)
		{
			l_sn[this->K +i] = Y_N[this->K*3 + 2*i +0];
			l_pn[this->K +i] = Y_N[this->K*3 + 2*i +1];
		}

		// tails bit in the interleaved domain
		for (auto i = 0; i < tail_i/2; i++)
		{
			l_si[this->K +i] = Y_N[this->K*3 + tail_n + 2*i +0];
			l_pi[this->K +i] = Y_N[this->K*3 + tail_n + 2*i +1];
		}
	}
	else // inter frame => input reordering
	{
		const auto n_frames = this->get_simd_inter_frame_level();

		for (auto i = 0; i < this->K; i++)
		{
			for (auto j = 0; j < n_frames; j++)
			{
				l_sn[i*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + i * 3 +0];
				l_pn[i*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + i * 3 +1];
				l_pi[i*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + i * 3 +2];
			}
		}
		pi.interleave(l_sn, l_si, true, this->get_simd_inter_frame_level());

		// tails bit in the natural domain
		for (auto i = 0; i < tail_n/2; i++)
		{
			for (auto j = 0; j < n_frames; j++)
			{
				l_sn[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + 2*i +0];
				l_pn[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + 2*i +1];
			}
		}

		// tails bit in the interleaved domain
		for (auto i = 0; i < tail_i/2; i++)
		{
			for (auto j = 0; j < n_frames; j++)
			{
				l_si[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + tail_n + 2*i +0];
				l_pi[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + tail_n + 2*i +1];
			}
		}
	}

	std::fill(l_e1n.begin(), l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::store(mipp::vector<B>& V_K) const
{
	if (this->get_simd_inter_frame_level() == 1)
	{
		V_K = s;
	}
	else // inter frame => output reordering
	{
		const auto n_frames = this->get_simd_inter_frame_level();

		std::vector<B*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = V_K.data() + f*this->K;
		Reorderer<B>::apply_rev(s.data(), frames, this->K);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_turbo<B_8,Q_8>;
template class Decoder_turbo<B_16,Q_16>;
template class Decoder_turbo<B_32,Q_32>;
template class Decoder_turbo<B_64,Q_64>;
#else
template class Decoder_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
