#include <chrono>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"

#include "Decoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_RA<B, R>
::Decoder_RA(const int& K, const int& N, const Interleaver<R>& interleaver, int max_iter, const int n_frames)
: Decoder          (K, N, n_frames, 1),
  Decoder_SIHO<B,R>(K, N, n_frames, 1),
  rep_count(N/K),
  max_iter(max_iter),
  Fw(N),
  Bw(N),
  Tu(N),
  Td(N),
  Wu(N),
  Wd(N),
  U(K),
  Xd(2),
  Xu(2),
  interleaver(interleaver)
{
	const std::string name = "Decoder_RA";
	this->set_name(name);

	if (max_iter <= 0)
	{
		std::stringstream message;
		message << "'max_iter' has to be greater than 0 ('max_iter' = " << max_iter << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N % K)
	{
		std::stringstream message;
		message << "'K' has to be a multiple of 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)interleaver.get_core().get_size() != N)
	{
		std::stringstream message;
		message << "'interleaver.get_core().get_size()' has to be equal to 'N' ('interleaver.get_core().get_size()' = "
		        << interleaver.get_core().get_size() << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	Xd[0].resize(N);
	Xd[1].resize(N);
	Xu[0].resize(N);
	Xu[1].resize(N);
}

template <typename B, typename R>
void Decoder_RA<B, R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	//set F, B and Td at 0
	for (auto i = 0; i < this->N; i++)
	{
		Fw[i] = 0;
		Bw[i] = 0;
		Td[i] = 0;
	}
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	for (auto iter = 0; iter < max_iter; iter++)
	{
		///////////////////
		// SISO decoding //
		///////////////////

		// Forward
		Fw[0] = Td[0];
		for (auto i = 1; i < this->N; i++)
			Fw[i] = check_node(Fw[i - 1] + Y_N[i - 1], Td[i]);

		// Backward
		Bw[this->N - 2] = check_node(Y_N[this->N - 1], Td[this->N - 1]);

		for (auto i = this->N - 3; i >= 0; i--)
			Bw[i] = check_node(Bw[i + 1] + Y_N[i + 1], Td[i + 1]);

		// Extrinsic
		Tu[0] = Bw[0] + Y_N[0];
		Tu[this->N - 1] = check_node(Y_N[this->N - 1], Y_N[this->N - 2] + Fw[this->N - 2]);
		for (auto i = 1; i < this->N - 1; i++)
			Tu[i] = check_node(Fw[i - 1] + Y_N[i - 1], Bw[i] + Y_N[i]);

		// Deinterleave
		interleaver.deinterleave(Tu.data(), Wu.data(), frame_id);

		// U computation
		R tmp;
		for (auto i = 0; i < this->K; i++)
		{
			tmp = 0;
			for (auto j = 0; j < rep_count; j++)
				tmp += Wu[i * rep_count + j];
			for (auto j = 0; j < rep_count; j++)
				Wd[i * rep_count + j] = tmp - Wu[i * rep_count + j];
			U[i] = tmp;
		}

		// Interleaving
		interleaver.interleave(Wd.data(), Td.data(), frame_id);
	}
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(U.data(), V_K, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
}

template <typename B, typename R>
R Decoder_RA<B, R>
::check_node(R a, R b)
{
	B sign1, sign2, resSign;
	R resMag, res;
	R mag1, mag2;

	sign1 = sign(a);
	sign2 = sign(b);
	mag1 = std::abs(a);
	mag2 = std::abs(b);
	resMag = std::min(mag1, mag2);
	resSign = (sign1 != sign2) ? -1 : 1;

	res = (resSign == 1) ? resMag : -resMag;

	return res;
}

template <typename B, typename R>
int Decoder_RA<B, R>
::sign(R x)
{
	return (x > 0) ? 1 : -1;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_RA<B_8,Q_8>;
template class aff3ct::module::Decoder_RA<B_16,Q_16>;
template class aff3ct::module::Decoder_RA<B_32,Q_32>;
template class aff3ct::module::Decoder_RA<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_RA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
