#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "Decoder_turbo_fast_CA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_turbo_fast_CA<B,R>
::Decoder_turbo_fast_CA(const int& K,
                        const int& N_without_tb,
                        const int& n_ite,
                        const Interleaver<int> &pi,
                        SISO<R> &siso_n,
                        SISO<R> &siso_i,
                        Scaling_factor<R> &scaling_factor,
                        CRC<B> &crc,
                        const bool buffered_encoding)
: Decoder_turbo_fast<B,R>(K, N_without_tb, n_ite, pi, siso_n, siso_i, scaling_factor, buffered_encoding), crc(crc)
{
	if (crc.size() > K)
		throw std::invalid_argument("aff3ct::module::Decoder_turbo_fast_CA: \"crc.size()\" has to be equal or "
		                            "smaller than K.");
}

template <typename B, typename R>
Decoder_turbo_fast_CA<B,R>
::~Decoder_turbo_fast_CA()
{
}

template <typename B, typename R>
void Decoder_turbo_fast_CA<B,R>
::_hard_decode(const R *Y_N, B *V_K)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	constexpr auto start_check_crc = 2;
	static_assert(start_check_crc >= 1, "");

	const auto n_frames = this->get_simd_inter_frame_level();
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
		std::copy(this->l_sn .begin() +  this->K             * n_frames,
		          this->l_sn .begin() + (this->K + tail_n_2) * n_frames,
		          this->l_sen.begin() +  this->K             * n_frames);

		// SISO in the natural domain
		this->siso_n.soft_decode(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), n_frames);

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

			check_crc = crc.check(this->s, this->get_simd_inter_frame_level());
		}

		if (!check_crc)
		{
			// apply the scaling factor
			this->scaling_factor(this->l_e2n, ite);

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
				this->scaling_factor(this->l_e2i, ite);
			else
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (auto i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
				{
					const auto r_post = mipp::Reg<R>(&this->l_e2i[i]) + mipp::Reg<R>(&this->l_sei[i]);
					r_post.store(&this->l_e2i[i]);
				}

			// make the deinterleaving
			this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1, n_frames);

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
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_fast_CA<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_fast_CA<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_fast_CA<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_fast_CA<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_fast_CA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
