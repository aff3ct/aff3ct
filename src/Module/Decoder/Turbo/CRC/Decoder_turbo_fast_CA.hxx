#include "Decoder_turbo_fast_CA.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

template <typename B, typename R>
Decoder_turbo_fast_CA<B,R>
::Decoder_turbo_fast_CA(const int& K,
                        const int& N_without_tb,
                        const int& n_ite,
                        const Interleaver<short> &pi,
                        SISO<R> &siso_n,
                        SISO<R> &siso_i,
                        Scaling_factor<R> &scaling_factor,
                        CRC<B> &crc,
                        const bool buffered_encoding)
: Decoder_turbo_fast<B,R>(K, N_without_tb, n_ite, pi, siso_n, siso_i, scaling_factor, buffered_encoding), crc(crc)
{
}

template <typename B, typename R>
Decoder_turbo_fast_CA<B,R>
::~Decoder_turbo_fast_CA()
{
}

template <typename B, typename R>
void Decoder_turbo_fast_CA<B,R>
::decode()
{
	constexpr auto start_check_crc = 3;

	assert(start_check_crc >= 1          );
	assert(start_check_crc <= this->n_ite);

	const auto n_frames = this->get_n_frames();
	const auto tail_n_2 = this->siso_n.tail_length() / 2;
	const auto tail_i_2 = this->siso_i.tail_length() / 2;

	// iterative turbo decoding process
	auto ite       = 1;
	auto check_crc = false;
	do
	{
		// l_se = sys + ext
		for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sen = mipp::Reg<R>(&this->l_sn[i]) + mipp::Reg<R>(&this->l_e1n[i]);
			r_l_sen.store(&this->l_sen[i]);
		}
		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i += mipp::nElReg<R>())
		{
			mipp::Reg<R> r_l_sen;
			r_l_sen.loadu (&this->l_sn [i]);
			r_l_sen.storeu(&this->l_sen[i]);
		}

		// SISO in the natural domain
		this->siso_n.decode(this->l_sen, this->l_pn, this->l_e2n);

		// the CRC is here because it is convenient to do not have to make the interleaving process!  
		if (ite >= start_check_crc)
		{
			// compute the hard decision (for the CRC)
			const auto loop_size1 = (this->K * n_frames) / mipp::nElReg<R>();
			for (auto i = 0; i < loop_size1; i++)
			{
				const auto r_ext  = mipp::Reg<R>(&this->l_e2n[i * mipp::nElReg<R>()]);
				const auto r_sys  = mipp::Reg<R>(&this->l_sen[i * mipp::nElReg<R>()]);
				const auto r_post = r_ext + r_sys;
#if defined(MIPP_NO_INTRINSICS) && defined(_MSC_VER) 
				const auto r_dec = mipp::Reg<B>((B)r_post.sign().r) >> (sizeof(B) * 8 - 1); // s[i] = (l_e2n[i] < 0);
#else
				const auto r_dec = mipp::Reg<B>(r_post.sign().r) >> (sizeof(B) * 8 - 1); // s[i] = (l_e2n[i] < 0);
#endif
				r_dec.store(&this->s[i * mipp::nElReg<R>()]);
			}
			const auto loop_size2 = this->K * n_frames;
			for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
				this->s[i] = (this->l_e2n[i] < 0);

			check_crc = crc.check(this->s);
		}

		if (!check_crc)
		{
			// apply the scaling factor
			this->scaling_factor(this->l_e2n, ite);

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
				mipp::Reg<R> r_l_sei;
				r_l_sei.loadu (&this->l_si [i]);
				r_l_sei.storeu(&this->l_sei[i]);
			}

			// SISO in the interleave domain
			this->siso_i.decode(this->l_sei, this->l_pi, this->l_e2i);

			if (ite != this->n_ite)
				// apply the scaling factor
				this->scaling_factor(this->l_e2i, ite);
			else
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
				{
					const auto r_post = mipp::Reg<R>(&this->l_e2i[i]) + mipp::Reg<R>(&this->l_sei[i]);
					r_post.store(&this->l_e2i[i]);
				}

			// make the deinterleaving
			this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1);

			// compute the hard decision only if we are in the last iteration
			if (ite == this->n_ite)
			{
				const auto loop_size1 = (this->K * n_frames) / mipp::nElReg<R>();
				for (auto i = 0; i < loop_size1; i++)
				{
					const auto r_post = mipp::Reg<R>(&this->l_e1n[i * mipp::nElReg<R>()]);
					// s[i] = (l_e1n[i] < 0);
#if defined(MIPP_NO_INTRINSICS) && defined(_MSC_VER) 
					const auto r_dec = mipp::Reg<B>((B)r_post.sign().r) >> (sizeof(B) * 8 - 1);
#else
					const auto r_dec = mipp::Reg<B>(r_post.sign().r) >> (sizeof(B) * 8 - 1);
#endif
					r_dec.store(&this->s[i * mipp::nElReg<R>()]);
				}
				const auto loop_size2 = this->K * n_frames;
				for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
					this->s[i] = (this->l_e1n[i] < 0);
			}
		}

		ite++; // increment the number of iteration
	}
	while ((ite <= this->n_ite) && !check_crc);
}