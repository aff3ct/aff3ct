#include <typeinfo>
#include <stdexcept>

#include "Modulator_BPSK.hpp"

using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modulator_BPSK<B,R,Q>
::Modulator_BPSK(const int N, const R sigma, const bool disable_sig2, const int n_frames, const std::string name)
: Modulator<B,R,Q>(N, n_frames, name),
  disable_sig2(disable_sig2), two_on_square_sigma((R)2.0 / (sigma * sigma))
{
}

template <typename B, typename R, typename Q>
Modulator_BPSK<B,R,Q>
::~Modulator_BPSK()
{
}

template <typename B, typename R, typename Q>
void Modulator_BPSK<B,R,Q>
::_modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	auto size = X_N1.size();
	for (unsigned i = 0; i < size; i++)
		X_N2[i] = (R)((B)1 - (X_N1[i] + X_N1[i])); // (X_N[i] == 1) ? -1 : +1
}

template <typename B, typename R, typename Q>
void Modulator_BPSK<B,R,Q>
::_demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"Q\" has to be float or double.");

	if (disable_sig2)
		Y_N2 = Y_N1;
	else
	{
		auto size = Y_N1.size();
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)two_on_square_sigma;
	}
}

template <typename B, typename R, typename Q>
void Modulator_BPSK<B,R,Q>
::_demodulate_with_gains(const mipp::vector<Q>& Y_N1, const mipp::vector<R>& H_N, mipp::vector<Q>& Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"Q\" has to be float or double.");

	if (disable_sig2)
	{
		auto size = Y_N1.size();
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)H_N[i];
	}
	else
	{
		auto size = Y_N1.size();
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)two_on_square_sigma * (Q)H_N[i];
	}
}

template <typename B, typename R, typename Q>
void Modulator_BPSK<B,R,Q>
::_demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"Q\" has to be float or double.");

	auto size = Y_N1.size();
	if (disable_sig2)
		for (unsigned i = 0; i < size; i++)
			Y_N3[i] = Y_N1[i] + Y_N2[i];
	else
		for (unsigned i = 0; i < size; i++)
			Y_N3[i] = (Y_N1[i] * (Q)two_on_square_sigma) + Y_N2[i];
}

template <typename B, typename R, typename Q>
void Modulator_BPSK<B,R,Q>
::_demodulate_with_gains(const mipp::vector<Q>& Y_N1, const mipp::vector<R>& H_N, const mipp::vector<Q>& Y_N2,
                               mipp::vector<Q>& Y_N3)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_BPSK: type \"Q\" has to be float or double.");

	auto size = Y_N1.size();
	if (disable_sig2)
		for (unsigned i = 0; i < size; i++)
			Y_N3[i] = Y_N1[i] * (Q)H_N[i] + Y_N2[i];
	else
		for (unsigned i = 0; i < size; i++)
			Y_N3[i] = (Y_N1[i] * (Q)H_N[i] * (Q)two_on_square_sigma) + Y_N2[i];
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modulator_BPSK<B_8,R_8,R_8>;
template class aff3ct::module::Modulator_BPSK<B_8,R_8,Q_8>;
template class aff3ct::module::Modulator_BPSK<B_16,R_16,R_16>;
template class aff3ct::module::Modulator_BPSK<B_16,R_16,Q_16>;
template class aff3ct::module::Modulator_BPSK<B_32,R_32,R_32>;
template class aff3ct::module::Modulator_BPSK<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modulator_BPSK<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modulator_BPSK<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
