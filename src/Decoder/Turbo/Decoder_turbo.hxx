#include "Decoder_turbo.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "../../Tools/Reorderer/Reorderer.hpp"

template <typename B, typename R>
Decoder_turbo<B,R>
::Decoder_turbo(const int& K,
                const int& N,
                const int& n_ite,
                const Interleaver<short> &pi,
                SISO<R> &siso_n,
                SISO<R> &siso_i,
                Scaling_factor<R> &scaling_factor,
                const bool buffered_encoding,
                const std::string name)
: Decoder<B,R>(name),
  K(K),
  N(N),
  n_ite(n_ite),
  buffered_encoding(buffered_encoding),
  pi(pi),
  siso_n(siso_n),
  siso_i(siso_i),
  scaling_factor(scaling_factor),
  l_sn ((K       + (siso_n.tail_length() / 2)) * siso_n.get_n_frames() + mipp::nElReg<R>()),
  l_si ((K       + (siso_i.tail_length() / 2)) * siso_i.get_n_frames() + mipp::nElReg<R>()),
  l_sen((K       + (siso_n.tail_length() / 2)) * siso_n.get_n_frames() + mipp::nElReg<R>()),
  l_sei((K       + (siso_i.tail_length() / 2)) * siso_i.get_n_frames() + mipp::nElReg<R>()),
  l_pn (((N-K)/2 + (siso_n.tail_length() / 2)) * siso_n.get_n_frames() + mipp::nElReg<R>()),
  l_pi (((N-K)/2 + (siso_i.tail_length() / 2)) * siso_i.get_n_frames() + mipp::nElReg<R>()),
  l_e1n((K                                   ) * siso_n.get_n_frames() + mipp::nElReg<R>()),
  l_e2n((K                                   ) * siso_n.get_n_frames() + mipp::nElReg<R>()),
  l_e1i((K                                   ) * siso_i.get_n_frames() + mipp::nElReg<R>()),
  l_e2i((K                                   ) * siso_i.get_n_frames() + mipp::nElReg<R>()),
  s    ((K                                   ) * siso_n.get_n_frames()                    )
{
	assert(siso_n.get_n_frames() == siso_i.get_n_frames());
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
	
	const auto p_size = (N - K) / 2; // size of the parity
	if (this->get_n_frames() == 1)
	{
		std::copy(Y_N.begin()               , Y_N.begin() + K           , l_sn.begin());
		std::copy(Y_N.begin() + K           , Y_N.begin() + K + 1*p_size, l_pn.begin());
		std::copy(Y_N.begin() + K + 1*p_size, Y_N.begin() + K + 2*p_size, l_pi.begin());
		pi.interleave(l_sn, l_si);

		// tails bit in the natural domain
		std::copy(Y_N.begin() + N           , Y_N.begin() + N + tail_n/2, l_pn.begin() +p_size);
		std::copy(Y_N.begin() + N + tail_n/2, Y_N.begin() + N + tail_n  , l_sn.begin() +p_size);

		// tails bit in the interleaved domain
		std::copy(Y_N.begin() + N + tail_n           , Y_N.begin() + N + tail_n + tail_i/2, l_pi.begin() +p_size);
		std::copy(Y_N.begin() + N + tail_n + tail_i/2, Y_N.begin() + N + tail_n + tail_i  , l_si.begin() +p_size);
	}
	else
	{
		const auto n_frames = this->get_n_frames();
		const auto frame_size = N + tail_n + tail_i;

		std::vector<const R*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size;
		Reorderer<R>::apply(frames, l_sn.data(), K);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +K;
		Reorderer<R>::apply(frames, l_pn.data(), p_size);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +K + p_size;
		Reorderer<R>::apply(frames, l_pi.data(), p_size);

		pi.interleave(l_sn, l_si, true);

		// tails bit in the natural domain
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N + tail_n/2;
		Reorderer<R>::apply(frames, &l_sn[K*n_frames], tail_n/2);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N;
		Reorderer<R>::apply(frames, &l_pn[p_size*n_frames], tail_n/2);

		// tails bit in the interleaved domain
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N + tail_n + tail_i/2;
		Reorderer<R>::apply(frames, &l_si[K*n_frames], tail_i/2);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +N + tail_n;
		Reorderer<R>::apply(frames, &l_pi[p_size*n_frames], tail_i/2);
	}
	std::fill(l_e1n.begin(), l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::standard_load(const mipp::vector<R>& Y_N)
{
	assert(K == (N - K) / 2);

	const auto tail_n = siso_n.tail_length();
	const auto tail_i = siso_i.tail_length();

	if (this->get_n_frames() == 1)
	{
		for (auto i = 0; i < K; i++)
		{
			l_sn[i] = Y_N[i*3 +0];
			l_pn[i] = Y_N[i*3 +1];
			l_pi[i] = Y_N[i*3 +2];
		}
		pi.interleave(l_sn, l_si);

		// tails bit in the natural domain
		for (auto i = 0; i < tail_n/2; i++)
		{
			l_sn[K +i] = Y_N[K*3 + 2*i +0];
			l_pn[K +i] = Y_N[K*3 + 2*i +1];
		}

		// tails bit in the interleaved domain
		for (auto i = 0; i < tail_i/2; i++)
		{
			l_si[K +i] = Y_N[K*3 + tail_n + 2*i +0];
			l_pi[K +i] = Y_N[K*3 + tail_n + 2*i +1];
		}
	}
	else // inter frame => input reordering
	{
		const auto n_frames = this->get_n_frames();

		for (auto i = 0; i < K; i++)
		{
			for (auto j = 0; j < n_frames; j++)
			{
				l_sn[i*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + i * 3 +0];
				l_pn[i*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + i * 3 +1];
				l_pi[i*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + i * 3 +2];
			}
		}
		pi.interleave(l_sn, l_si, true);

		// tails bit in the natural domain
		for (auto i = 0; i < tail_n/2; i++)
		{
			for (auto j = 0; j < n_frames; j++)
			{
				l_sn[(K +i)*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + K*3 + 2*i +0];
				l_pn[(K +i)*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + K*3 + 2*i +1];
			}
		}

		// tails bit in the interleaved domain
		for (auto i = 0; i < tail_i/2; i++)
		{
			for (auto j = 0; j < n_frames; j++)
			{
				l_si[(K +i)*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + K*3 + tail_n + 2*i +0];
				l_pi[(K +i)*n_frames +j] = Y_N[j*((K*3) + tail_n + tail_i) + K*3 + tail_n + 2*i +1];
			}
		}
	}

	std::fill(l_e1n.begin(), l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::store(mipp::vector<B>& V_K) const
{
	if (this->get_n_frames() == 1)
	{
		V_K = s;
	}
	else // inter frame => output reordering
	{
		const auto n_frames = this->get_n_frames();

		std::vector<B*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = V_K.data() + f*K;
		Reorderer<B>::apply_rev(s.data(), frames, K);
	}
}