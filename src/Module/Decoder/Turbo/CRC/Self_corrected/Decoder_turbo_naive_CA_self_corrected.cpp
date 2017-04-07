#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Math/utils.h"

#include "Decoder_turbo_naive_CA_self_corrected.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_turbo_naive_CA_self_corrected<B,R>
::Decoder_turbo_naive_CA_self_corrected(const int& K,
                                        const int&  N_without_tb,
                                        const int& n_ite,
                                        const Interleaver<int> &pi,
                                        SISO<R> &siso_n,
                                        SISO<R> &siso_i,
                                        Scaling_factor<R> &scaling_factor,
                                        CRC<B> &crc,
                                        const bool buffered_encoding)
: Decoder_turbo_naive_CA<B,R>(K, N_without_tb, n_ite, pi, siso_n, siso_i, scaling_factor, crc, buffered_encoding),
  ext_nat(n_ite, mipp::vector<R>(K * siso_n.get_simd_inter_frame_level() + mipp::nElReg<R>(), (R)0)),
  ext_int(n_ite, mipp::vector<R>(K * siso_i.get_simd_inter_frame_level() + mipp::nElReg<R>(), (R)0)),
  osc_nat                       (K * siso_n.get_simd_inter_frame_level() + mipp::nElReg<B>(), (B)0),
  osc_int                       (K * siso_i.get_simd_inter_frame_level() + mipp::nElReg<B>(), (B)0),
  previously_corrected_nat      (K * siso_n.get_simd_inter_frame_level() + mipp::nElReg<B>(), (B)0),
  previously_corrected_int      (K * siso_i.get_simd_inter_frame_level() + mipp::nElReg<B>(), (B)0)
{
}

template <typename B, typename R>
Decoder_turbo_naive_CA_self_corrected<B,R>
::~Decoder_turbo_naive_CA_self_corrected()
{
}

template <typename B, typename R>
void Decoder_turbo_naive_CA_self_corrected<B,R>
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
		// sys + ext
		for (auto i = 0; i < this->K * n_frames; i++)
			this->l_sen[i] = this->l_sn[i] + this->l_e1n[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i++)
			this->l_sen[i] = this->l_sn[i];

		// SISO in the natural domain
		this->siso_n.soft_decode(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), n_frames);

		// the CRC is here because it is convenient to do not have to make the interleaving process!
		if (ite >= start_check_crc)
		{
			// compute the hard decision (for the CRC)
			for (auto i = 0; i < this->K * n_frames; i++)
				this->s[i] = (this->l_e2n[i] + this->l_sen[i]) < 0;

			check_crc = this->crc.check(this->s, this->get_simd_inter_frame_level());
		}

		if (!check_crc)
		{
			// apply the scaling factor
			this->scaling_factor(this->l_e2n, ite);

			// make the interleaving
			this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1, n_frames);

			// self corrected
			this->collect(this->l_e1i, ite, this->ext_nat);
			if (ite > 1)
			{
				this->calc_osc(this->ext_nat,  ite, this->osc_nat);
				if (ite > 3)
					this->correct(this->osc_nat, this->previously_corrected_nat, this->l_e1i);
			}

			// sys + ext
			for (auto i = 0; i < this->K * n_frames; i++)
				this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

			for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i++)
				this->l_sei[i] = this->l_si[i];

			// SISO in the interleave domain
			this->siso_i.soft_decode(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), n_frames);

			if (ite != this->n_ite)
				// apply the scaling factor
				this->scaling_factor(this->l_e2i, ite);
			else
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (auto i = 0; i < this->K * n_frames; i++)
					this->l_e2i[i] += this->l_sei[i];

			// make the deinterleaving
			this->pi.deinterleave(this->l_e2i, this->l_e1n, n_frames > 1, n_frames);

			// compute the hard decision only if we are in the last iteration
			if (ite == this->n_ite)
				for (auto i = 0; i < this->K * n_frames; i++)
					this->s[i] = this->l_e1n[i] < 0;

			// self corrected
			this->collect(this->l_e1n, ite, this->ext_int);
			if (ite > 1)
			{
				this->calc_osc(this->ext_int,  ite, this->osc_int);
				if (ite > 3)
					this->correct(this->osc_int, this->previously_corrected_int, this->l_e1n);
			}
		}

		ite++; // increment the number of iteration
	}
	while ((ite <= this->n_ite) && !check_crc);
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now();// --------------------------------------------------------- STORE
	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

template <typename B, typename R>
inline void Decoder_turbo_naive_CA_self_corrected<B,R>
::collect(const mipp::vector<R> &cur_ext, const int &ite, std::vector<mipp::vector<R>> &ext_hist)
{
	// ite starts at 1 in tdec process
	ext_hist[ite -1] = cur_ext;
}

template <typename B, typename R>
inline void Decoder_turbo_naive_CA_self_corrected<B,R>
::calc_osc(const std::vector<mipp::vector<R>> &ext_hist, const int &ite, mipp::vector<B> &osc)
{
	B sgn_cur, sgn_prev;
	for (auto idx = 0; idx < (int)osc.size(); idx++)
	{
		sgn_cur  = sgn<B,R>(ext_hist[ite-1][idx]);
		sgn_prev = sgn<B,R>(ext_hist[ite-2][idx]);
		osc[idx] = sgn_cur != sgn_prev;
	}
}

template <typename B, typename R>
inline void Decoder_turbo_naive_CA_self_corrected<B,R>
::correct(const mipp::vector<B> &osc, mipp::vector<B> &prev_corr, mipp::vector<R> &ext)
{
	for (auto idx = 0; idx < (int)ext.size(); idx++)
	{
		if (osc[idx] && !prev_corr[idx])
		{
			ext[idx]       = (R)0;
			prev_corr[idx] = (B)1;
		}
		else
			prev_corr[idx] = (B)0;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_naive_CA_self_corrected<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_naive_CA_self_corrected<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_naive_CA_self_corrected<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_naive_CA_self_corrected<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_naive_CA_self_corrected<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
