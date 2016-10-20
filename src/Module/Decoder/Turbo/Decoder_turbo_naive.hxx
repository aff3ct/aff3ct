#include "Decoder_turbo_naive.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

template <typename B, typename R>
Decoder_turbo_naive<B,R>
::Decoder_turbo_naive(const int& K,
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
Decoder_turbo_naive<B,R>
::~Decoder_turbo_naive()
{
}

template <typename B, typename R>
void Decoder_turbo_naive<B,R>
::decode()
{
	const auto n_frames = this->get_n_frames();
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
		this->siso_n.decode(this->l_sen, this->l_pn, this->l_e2n);

		// apply the scaling factor
		this->scaling_factor(this->l_e2n, 2 * (ite -1));

		// make the interleaving
		this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1);

		// sys + ext
		for (auto i = 0; i < this->K * n_frames; i++) 
			this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i++) 
			this->l_sei[i] = this->l_si[i];

		// SISO in the interleave domain
		this->siso_i.decode(this->l_sei, this->l_pi, this->l_e2i);

		if (ite != this->n_ite)
			// apply the scaling factor
			this->scaling_factor(this->l_e2i, 2 * (ite -1) +1);
		else
			// add the systematic information to the extrinsic information, gives the a posteriori information
			for (auto i = 0; i < this->K * n_frames; i++)
				this->l_e2i[i] += this->l_sei[i];

		// make the deinterleaving
		this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1);
	}

	// take the hard decision
	for (auto i = 0; i < this->K * n_frames; i++)
		this->s[i] = this->l_e1n[i] < 0;
}
