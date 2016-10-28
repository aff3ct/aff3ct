#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Decoder_turbo_naive_CA.hpp"

template <typename B, typename R>
Decoder_turbo_naive_CA<B,R>
::Decoder_turbo_naive_CA(const int& K,
                         const int& N_without_tb,
                         const int& n_ite,
                         const Interleaver<short> &pi,
                         SISO<R> &siso_n,
                         SISO<R> &siso_i,
                         Scaling_factor<R> &scaling_factor,
                         CRC<B> &crc,
                         const bool buffered_encoding)
: Decoder_turbo_naive<B,R>(K, N_without_tb, n_ite, pi, siso_n, siso_i, scaling_factor, buffered_encoding), crc(crc)
{
}

template <typename B, typename R>
Decoder_turbo_naive_CA<B,R>
::~Decoder_turbo_naive_CA()
{
}

template <typename B, typename R>
void Decoder_turbo_naive_CA<B,R>
::hard_decode()
{
	constexpr auto start_check_crc = 2;

	assert(start_check_crc >= 1          );
	assert(start_check_crc <= this->n_ite);

	const auto n_frames = this->get_simd_inter_frame_level();
	const auto tail_n_2 = this->siso_n.tail_length() / 2;
	const auto tail_i_2 = this->siso_i.tail_length() / 2;

	// iterative turbo decoding process
	auto ite       = 1;
	auto check_crc = false;
	do
	{
		// sys + ext
		for (auto i = 0; i < this->K * n_frames; i++) 
			this->l_sen[i] = this->l_sn[i] + this->l_e1n[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i++) 
			this->l_sen[i] = this->l_sn[i];

		// SISO in the natural domain
		this->siso_n.soft_decode(this->l_sen, this->l_pn, this->l_e2n);

		// the CRC is here because it is convenient to do not have to make the interleaving process!  
		if (ite >= start_check_crc)
		{
			// compute the hard decision (for the CRC)
			for (auto i = 0; i < this->K * n_frames; i++)
				this->s[i] = (this->l_e2n[i] + this->l_sen[i]) < 0;

			check_crc = crc.check(this->s);
		}

		if (!check_crc)
		{
			// apply the scaling factor
			this->scaling_factor(this->l_e2n, ite);

			// make the interleaving
			this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1, this->get_simd_inter_frame_level());

			// sys + ext
			for (auto i = 0; i < this->K * n_frames; i++)
				this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

			for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i++)
				this->l_sei[i] = this->l_si[i];

			// SISO in the interleave domain
			this->siso_i.soft_decode(this->l_sei, this->l_pi, this->l_e2i);

			if (ite != this->n_ite)
				// apply the scaling factor
				this->scaling_factor(this->l_e2i, ite);
			else
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (auto i = 0; i < this->K * n_frames; i++)
					this->l_e2i[i] += this->l_sei[i];

			// make the deinterleaving
			this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1, this->get_simd_inter_frame_level());

			// compute the hard decision only if we are in the last iteration
			if (ite == this->n_ite)
				for (auto i = 0; i < this->K * n_frames; i++)
					this->s[i] = this->l_e1n[i] < 0;
		}

		ite++; // increment the number of iteration
	}
	while ((ite <= this->n_ite) && !check_crc);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_turbo_naive_CA<B_8,Q_8>;
template class Decoder_turbo_naive_CA<B_16,Q_16>;
template class Decoder_turbo_naive_CA<B_32,Q_32>;
template class Decoder_turbo_naive_CA<B_64,Q_64>;
#else
template class Decoder_turbo_naive_CA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
