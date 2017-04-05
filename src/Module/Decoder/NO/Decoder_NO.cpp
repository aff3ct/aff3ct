#include <chrono>

#include "Decoder_NO.hpp"

using namespace aff3ct::module;

template <typename B, typename R>
Decoder_NO<B,R>
::Decoder_NO(const int K, const int N, const int n_frames, const std::string name)
: Decoder_SISO<B,R>(K, N, n_frames, 1, name)
{
	if (N != K)
		throw std::invalid_argument("aff3ct::module::Decoder_NO: \"K\" and \"N\" have to be equal.");
}

template <typename B, typename R>
Decoder_NO<B,R>
::~Decoder_NO()
{
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_hard_decode_fbf(const R *Y_N, B *V_K)
{
	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	auto vec_loop_size = (this->K / mipp::nElReg<R>()) * mipp::nElReg<R>();
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const auto r_Y_N = mipp::Reg<R>(&Y_N[i]);

		// s[i] = Y_N[i] < 0;
#if defined(MIPP_NO_INTRINSICS) && defined(_MSC_VER)
		const auto r_s = mipp::Reg<B>((B)r_Y_N.sign().r) >> (sizeof(B) * 8 - 1);
#else
		const auto r_s = mipp::Reg<B>(r_Y_N.sign().r) >> (sizeof(B) * 8 - 1);
#endif
		r_s.store(&V_K[i]);
	}
	for (auto i = vec_loop_size; i < this->K; i++)
		V_K[i] = Y_N[i] < 0;
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_store_total += d_store;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_soft_decode_fbf(const R *sys, const R *par, R *ext)
{
	std::copy(sys, sys + this->K, ext);
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_soft_decode_fbf(const R *Y_N1, R *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N, Y_N2);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_NO<B_8,Q_8>;
template class aff3ct::module::Decoder_NO<B_16,Q_16>;
template class aff3ct::module::Decoder_NO<B_32,Q_32>;
template class aff3ct::module::Decoder_NO<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
