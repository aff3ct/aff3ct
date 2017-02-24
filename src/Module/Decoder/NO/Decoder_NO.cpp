#include "Tools/Display/bash_tools.h"

#include "Decoder_NO.hpp"
using namespace aff3ct;

template <typename B, typename R>
Decoder_NO<B,R>
::Decoder_NO(const int K, const int N, const int n_frames, const std::string name)
: Decoder<B,R>(K, N, n_frames, 1, name          ),
  SISO   <  R>(K, N, n_frames, 1, name + "_siso"),
  Y_N(N)
{
}

template <typename B, typename R>
Decoder_NO<B,R>
::~Decoder_NO()
{
}

template <typename B, typename R>
void Decoder_NO<B,R>
::load(const mipp::vector<R>& Y_N)
{
	assert(Y_N.size() >= this->Y_N.size());
	std::copy(Y_N.begin(), Y_N.begin() + this->Y_N.size(), this->Y_N.begin());
}

template <typename B, typename R>
void Decoder_NO<B,R>
::hard_decode()
{
}

template <typename B, typename R>
void Decoder_NO<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(Y_N.size() <= V_K.size());

	auto K = (int) Y_N.size();

	// take the hard decision
	auto vec_loop_size = (K / mipp::nElReg<R>()) * mipp::nElReg<R>();
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
	for (auto i = vec_loop_size; i < K; i++)
		V_K[i] = Y_N[i] < 0;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	assert(sys.size() == ext.size());

	ext = sys;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	Y_N2 = Y_N1;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_NO<B_8,Q_8>;
template class Decoder_NO<B_16,Q_16>;
template class Decoder_NO<B_32,Q_32>;
template class Decoder_NO<B_64,Q_64>;
#else
template class Decoder_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
