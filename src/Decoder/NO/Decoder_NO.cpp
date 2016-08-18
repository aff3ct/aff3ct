#include "Decoder_NO.hpp"

template <typename B, typename R>
Decoder_NO<B,R>
::Decoder_NO(const int K, const int N, std::string name)
: Decoder<B,R>(K, N, name.c_str())
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
	this->Y_N = Y_N;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::decode()
{
}

template <typename B, typename R>
void Decoder_NO<B,R>
::store(mipp::vector<B>& V_K) const
{
	assert(Y_N.size() == V_K.size());

	auto K = (int) Y_N.size();

	// take the hard decision
	auto vec_loop_size = (K / mipp::nElReg<R>()) * mipp::nElReg<R>();
	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const auto r_Y_N = mipp::Reg<R>(&Y_N[i]);

		// s[i] = Y_N[i] < 0;
		const auto r_s = mipp::Reg<B>(r_Y_N.sign().r) >> (sizeof(B) * 8 -1);
		r_s.store(&V_K[i]);
	}
	for (auto i = vec_loop_size; i < K; i++)
		V_K[i] = Y_N[i] < 0;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	assert(sys.size() == ext.size());

	ext = sys;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	Y_N2 = Y_N1;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_NO<B_8,Q_8>;
template class Decoder_NO<B_16,Q_16>;
template class Decoder_NO<B_32,Q_32>;
template class Decoder_NO<B_64,Q_64>;
#else
template class Decoder_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation