#include <cassert>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Decoder_repetition.hpp"

template <typename B, typename R>
Decoder_repetition<B,R>
::Decoder_repetition(const int& K, const int& N, const bool buffered_encoding, const int n_frames,
                     const std::string name)
 : Decoder<B,R>(K, N, n_frames, 1, name),
   SISO   <  R>(K, N, n_frames, 1, name + "_siso"),
   rep_count((N/K) -1), buffered_encoding(buffered_encoding), sys(K), par(K * rep_count), ext(K), s(K)
{
	assert(N % K == 0);
}

template <typename B, typename R>
Decoder_repetition<B,R>
::~Decoder_repetition()
{
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::load(const mipp::vector<R>& Y_N)
{
	if (!buffered_encoding)
	{
		for (auto i = 0; i < this->K; i++)
		{
			const auto off1 = i * (rep_count +1);
			const auto off2 = off1 +1;

			sys[i] = Y_N[off1];
			for (auto j = 0; j < rep_count; j++)
				par[j*this->K +i] = Y_N[off2 +j];
		}
	}
	else
	{
		std::copy(Y_N.begin(), Y_N.begin() + this->K, sys.begin());
		for (auto i = 0; i < rep_count; i++)
			std::copy(Y_N.begin() + (i +1) * this->K, Y_N.begin() + (i +2) * this->K, par.begin() + (i +0) * this->K);
	}
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::hard_decode()
{
	soft_decode(sys, par, ext);

	// take the hard decision
	auto vec_loop_size = (this->K / mipp::nElReg<R>()) * mipp::nElReg<R>();
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const auto r_ext = mipp::Reg<R>(&ext[i]);

		// s[i] = ext[i] < 0;
#if defined(MIPP_NO_INTRINSICS) && defined(_MSC_VER) 
		const auto r_s = mipp::Reg<B>((B)r_ext.sign().r) >> (sizeof(B) * 8 - 1);
#else
		const auto r_s = mipp::Reg<B>(r_ext.sign().r) >> (sizeof(B) * 8 - 1);
#endif
		r_s.store(&s[i]);
	}
	for (auto i = vec_loop_size; i < this->K; i++)
		s[i] = ext[i] < 0;
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::store(mipp::vector<B>& V_K) const
{
	V_K = s;
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::_soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_repetition<B_8,Q_8>;
template class Decoder_repetition<B_16,Q_16>;
template class Decoder_repetition<B_32,Q_32>;
template class Decoder_repetition<B_64,Q_64>;
#else
template class Decoder_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
