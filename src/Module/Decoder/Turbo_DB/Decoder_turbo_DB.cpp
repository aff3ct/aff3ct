#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Decoder_turbo_DB.hpp"
#include "Tools/Math/utils.h"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo_DB<B,R>
::Decoder_turbo_DB(const int& K,
                   const int& N,
                   const int& n_ite,
                   const Interleaver<R> &pi,
                   Decoder_RSC_DB_BCJR<B,R> &siso_n,
                   Decoder_RSC_DB_BCJR<B,R> &siso_i)
: Decoder          (K, N, siso_n.get_n_frames(), 1),
  Decoder_SIHO<B,R>(K, N, siso_n.get_n_frames(), 1),
  n_ite            (n_ite),
  pi               (pi),
  siso_n           (siso_n),
  siso_i           (siso_i),
  l_cpy            (2 * K),
  l_sn             (2 * K),
  l_si             (2 * K),
  l_sen            (2 * K),
  l_sei            (2 * K),
  l_pn             (    K),
  l_pi             (    K),
  l_e1n            (2 * K),
  l_e2n            (2 * K),
  l_e1i            (2 * K),
  l_e2i            (2 * K),
  s                (    K)
{
	const std::string name = "Decoder_turbo_DB";
	this->set_name(name);

	if (K % 2)
	{
		std::stringstream message;
		message << "'K' has to be a divisible by 2 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_n.get_K() != K)
	{
		std::stringstream message;
		message << "'siso_n.get_K()' has to be equal to 'K' ('siso_n.get_K()' = " << siso_n.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_i.get_K() != K)
	{
		std::stringstream message;
		message << "'siso_i.get_K()' has to be equal to 'K' ('siso_i.get_K()' = " << siso_i.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() * 2 != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' * 2 has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_n.get_n_frames() != siso_i.get_n_frames())
	{
		std::stringstream message;
		message << "'siso_n.get_n_frames()' has to be equal to 'siso_i.get_n_frames()' ('siso_n.get_n_frames()' = "
		        << siso_n.get_n_frames() << ", 'siso_i.get_n_frames()' = " << siso_i.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_n.get_simd_inter_frame_level() != 1)
	{
		std::stringstream message;
		message << "'siso_n.get_simd_inter_frame_level()' has to be equal to 1 "
		        << "('siso_n.get_simd_inter_frame_level()' = " << siso_n.get_simd_inter_frame_level() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_n.get_simd_inter_frame_level() != siso_i.get_simd_inter_frame_level())
	{
		std::stringstream message;
		message << "'siso_n.get_simd_inter_frame_level()' has to be equal to 'siso_i.get_simd_inter_frame_level()' "
		        << "('siso_n.get_simd_inter_frame_level()' = " << siso_n.get_simd_inter_frame_level()
		        << ", 'siso_i.get_simd_inter_frame_level()' = " << siso_i.get_simd_inter_frame_level() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
void Decoder_turbo_DB<B,R>
::add_handler_siso_n(std::function<bool(const int,
                                        const mipp::vector<R>&,
                                              mipp::vector<R>&,
                                              mipp::vector<B>&)> callback)
{
	this->callbacks_siso_n.push_back(callback);
}

template <typename B, typename R>
void Decoder_turbo_DB<B,R>
::add_handler_siso_i(std::function<bool(const int, const mipp::vector<R>&, mipp::vector<R>&)> callback)
{
	this->callbacks_siso_i.push_back(callback);
}

template <typename B, typename R>
void Decoder_turbo_DB<B,R>
::add_handler_end(std::function<void(const int)> callback)
{
	this->callbacks_end.push_back(callback);
}

template <typename B, typename R>
void Decoder_turbo_DB<B,R>
::_load(const R *Y_N)
{
	this->siso_n.notify_new_frame();
	this->siso_i.notify_new_frame();

	auto j = 0;
	for (auto i = 0; i < this->K/2; i++)
	{
		R a = tools::div2(Y_N[j++]);
		R b = tools::div2(Y_N[j++]);
		this->l_sn[4*i + 0] =  a + b;
		this->l_sn[4*i + 1] =  a - b;
		this->l_sn[4*i + 2] = -a + b;
		this->l_sn[4*i + 3] = -a - b;
	}

	for (auto i = 0; i < this->K; i+=2)
	{
		this->l_pn[i] = tools::div2(Y_N[j++]);
		this->l_pi[i] = tools::div2(Y_N[j++]);
	}

	for (auto i = 1; i < this->K; i+=2)
	{
		this->l_pn[i] = tools::div2(Y_N[j++]);
		this->l_pi[i] = tools::div2(Y_N[j++]);
	}

	// make the interleaving to get l_si (2 steps interleaving)
	l_cpy = this->l_sn;
	for (auto i = 0; i < 2 * this->K; i += 8)
		std::swap(l_cpy[i+1], l_cpy[i+2]);
	for (auto i = 0; i < this->K; i += 2)
	{
		const auto l = pi.get_core().get_lut_inv()[i >> 1];
		for (auto bps = 0; bps < 2; bps++) this->l_si[2 * (i +0) + bps] = l_cpy[(4 * l + 0) + bps];
		for (auto bps = 0; bps < 2; bps++) this->l_si[2 * (i +1) + bps] = l_cpy[(4 * l + 2) + bps];
	}

	std::fill(this->l_e1n.begin(), this->l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo_DB<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	const auto n_frames = this->get_simd_inter_frame_level();

	// iterative turbo decoding process
	bool stop = false;
	auto ite  = 1;
	do
	{
		// sys + ext
		for (auto i = 0; i < 2 * this->K; i++)
			this->l_sen[i] = this->l_sn[i] + this->l_e1n[i];

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
			l_cpy = this->l_e2n;
			for (auto i = 0; i < 2 * this->K; i += 8)
				std::swap(l_cpy[i+1], l_cpy[i+2]);
			for (auto i = 0; i < this->K; i += 2)
			{
				const auto l = pi.get_core().get_lut_inv()[i >> 1];
				for (auto bps = 0; bps < 2; bps++) this->l_e1i[2 * (i +0) + bps] = l_cpy[(4 * l + 0) + bps];
				for (auto bps = 0; bps < 2; bps++) this->l_e1i[2 * (i +1) + bps] = l_cpy[(4 * l + 2) + bps];
			}

			// sys + ext
			for (auto i = 0; i < 2 * this->K; i++)
				this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

			// SISO in the interleaved domain
			this->siso_i.decode_siso(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), n_frames);

			for (auto cb : this->callbacks_siso_i)
			{
				stop = cb(ite, this->l_sei, this->l_e2i);
				if (stop) break;
			}

			if (ite == this->n_ite || stop)
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (auto i = 0; i < 2 * this->K; i++)
					this->l_e2i[i] += this->l_sei[i];

			// make the deinterleaving
			for (auto i = 0; i < this->K; i += 2)
			{
				const auto l = pi.get_core().get_lut()[i >> 1];
				for (auto bps = 0; bps < 2; bps++) this->l_e1n[2 * (i +0) + bps] = l_e2i[(4 * l + 0) + bps];
				for (auto bps = 0; bps < 2; bps++) this->l_e1n[2 * (i +1) + bps] = l_e2i[(4 * l + 2) + bps];
			}
			for (auto i = 0; i < 2 * this->K; i += 8)
				std::swap(this->l_e1n[i+1], this->l_e1n[i+2]);

			// compute the hard decision only if we are in the last iteration
			if (ite == this->n_ite || stop)
			{
				for (auto i = 0; i < this->K; i += 2)
				{
					this->s[i  ] = (std::max(this->l_e1n[2*i+2], this->l_e1n[2*i+3]) -
					                std::max(this->l_e1n[2*i+0], this->l_e1n[2*i+1])) > 0;
					this->s[i+1] = (std::max(this->l_e1n[2*i+1], this->l_e1n[2*i+3]) -
					                std::max(this->l_e1n[2*i+0], this->l_e1n[2*i+2])) > 0;
				}
			}
		}
		ite++; // increment the number of iteration
	}
	while ((ite <= this->n_ite) && !stop);

	for (auto cb : this->callbacks_end)
		cb(ite -1);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
void Decoder_turbo_DB<B,R>
::_store(B *V_K) const
{
	std::copy(s.data(), s.data() + this->K, V_K);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_DB<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_DB<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_DB<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_DB<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
