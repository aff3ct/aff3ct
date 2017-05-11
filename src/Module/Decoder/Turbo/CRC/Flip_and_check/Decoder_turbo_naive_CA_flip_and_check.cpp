#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Math/utils.h"

#include "Decoder_turbo_naive_CA_flip_and_check.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R>
mipp::vector<unsigned int> partial_sort_indexes(const mipp::vector<R> &v)
{
	// initialize original index locations
	mipp::vector<unsigned int> idx(v.size());
	for (size_t i = 0; i != idx.size(); ++i)
		idx[i] = (unsigned int)i;
	// sort indexes based on comparing values in v
	std::partial_sort(idx.begin(),idx.begin() + 20, idx.end(),
	                  [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});
	return idx;
}

template <typename B, typename R>
Decoder_turbo_naive_CA_flip_and_check<B,R>
::Decoder_turbo_naive_CA_flip_and_check(const int& K,
                                        const int& N,
                                        const int& n_ite,
                                        const Interleaver<int> &pi,
                                        SISO<R> &siso_n,
                                        SISO<R> &siso_i,
                                        Scaling_factor<R> &scaling_factor,
                                        CRC<B> &crc,
                                        int fnc_q, int fnc_m, int fnc_M, int fnc_s,
                                        const bool buffered_encoding)
: Decoder_turbo_naive_CA<B,R>(K, N, n_ite, pi, siso_n, siso_i, scaling_factor, crc, buffered_encoding),
  q(fnc_q), nb_patterns((1<<q)-1),
  metric(K, (R)0),
  tab_flips(nb_patterns, mipp::vector<B>(q, (B)0)),
  s_tmp((K) * siso_n.get_simd_inter_frame_level())
{
	// generation of the array that will be used to flip the bits
	// it contains: 1000, 0100, 1100, 0010, ...
	int b10, l;
	for (int i=1; i<=nb_patterns; i++)
	{
		b10 = i; // number in base 10
		l   = 0;
		while (b10 > 0)
		{
			tab_flips[i-1][l] = (b10 % 2);
			b10 /=2;
			l++;
		}
	}

	// fnc_ite contains the iterations for which the FNC will be performed
	int ite = fnc_m;
	do
	{
		fnc_ite.push_back(ite);
		ite += fnc_s;
	}
	while (ite <= fnc_M);
}

template <typename B, typename R>
Decoder_turbo_naive_CA_flip_and_check<B,R>
::~Decoder_turbo_naive_CA_flip_and_check()
{
}

template <typename B, typename R>
void Decoder_turbo_naive_CA_flip_and_check<B,R>
::_hard_decode(const R *Y_N, B *V_K)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
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
		this->siso_n.soft_decode(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), n_frames);

		// the CRC is here because it is convenient to do not have to make the interleaving process!
		if (ite >= start_check_crc)
		{
			// compute the hard decision (for the CRC)
			for (auto i = 0; i < this->K * n_frames; i++)
				this->s[i] = (this->l_e2n[i] + this->l_sen[i]) < 0;

			check_crc = this->crc.check(this->s, n_frames);
		}

		if (!check_crc && std::binary_search(fnc_ite.begin(), fnc_ite.end(), ite))
			check_crc = apply_flip_and_check(this->l_e2n, this->l_sen, this->s);

		if (!check_crc)
		{
			// apply the scaling factor
			this->scaling_factor(this->l_e2n, ite);

			// make the interleaving
			this->pi.interleave(this->l_e2n, this->l_e1i, n_frames > 1, n_frames);

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
bool Decoder_turbo_naive_CA_flip_and_check<B,R>
::apply_flip_and_check(const mipp::vector<R>& l_en, const mipp::vector<R>& l_sen, mipp::vector<B>& s)
{
	// reconstruct the a posteriori information and calculate the metric associated
	for (auto i = 0; i < this->K; i++)
		metric[i] = std::abs((div2(l_en[i]) + div4(l_en[i]) + l_sen[i]));

	// get the least reliable positions
	mipp::vector<unsigned int> positions = partial_sort_indexes(metric);

	// test all patterns by xoring with tab_flib
	bool check_crc = false;
	auto pattern = 0;
	do
	{
		std::copy(s.begin(), s.end(), s_tmp.begin());
		for (auto depth = 0; depth < q; depth++)
			s_tmp[positions[depth]] ^= tab_flips[pattern][depth];
		check_crc = this->crc.check(s_tmp, this->get_simd_inter_frame_level());
		pattern++;
	}
	while ((pattern < nb_patterns) && !check_crc );

	// if the crc is verified, replace the decoder's output
	if (check_crc)
		std::copy(s_tmp.begin(), s_tmp.end(), s.begin());

	return check_crc;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_turbo_naive_CA_flip_and_check<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_naive_CA_flip_and_check<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_naive_CA_flip_and_check<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_naive_CA_flip_and_check<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_naive_CA_flip_and_check<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
