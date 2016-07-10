#include "../../Tools/bash_tools.h"
#include "Modulator_BPSK_fast.hpp"

template <typename B, typename R>
Modulator_BPSK_fast<B,R>
::Modulator_BPSK_fast()
{
}

template <typename B, typename R>
Modulator_BPSK_fast<B,R>
::~Modulator_BPSK_fast()
{
}

template <typename B, typename R>
void Modulator_BPSK_fast<B,R>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const
{
	std::cerr << bold_red("(EE) The fast modulator does not support this type of data.") << std::endl;
	std::exit(-1);
}

template <>
void Modulator_BPSK_fast<int, float>
::modulate(const mipp::vector<int>& X_N1, mipp::vector<float>& X_N2) const
{
	auto size = X_N1.size();
	
	const auto vec_loop_size = (size / mipp::nElReg<int>()) * mipp::nElReg<int>();
	const mipp::Reg<float> one = 1.f;
	for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<int>())
	{
		auto x1b = mipp::Reg<int>(&X_N1[i]);
		auto x1r = x1b.cvt<float>();
		auto x2r = one - (x1r + x1r);
		x2r.store(&X_N2[i]);
	}

	for (unsigned i = vec_loop_size; i < size; i++)
		X_N2[i] = 1 - (X_N1[i] + X_N1[i]); // (X_N[i] == 1) ? -1 : +1
}

template <>
void Modulator_BPSK_fast<short, float>
::modulate(const mipp::vector<short>& X_N1, mipp::vector<float>& X_N2) const
{
	auto size = X_N1.size();
	
	const auto vec_loop_size = size / mipp::nElReg<short>();
	const mipp::Reg<float> one = 1.f;
	for (unsigned i = 0; i < vec_loop_size; i++)
	{
		auto x1b = mipp::Reg<short>(&X_N1[i * mipp::nElReg<short>()]);
		auto x1b_low  = x1b.low ().cvt<int>();
		auto x1b_high = x1b.high().cvt<int>();

		auto x1r_low  = x1b_low .cvt<float>();
		auto x1r_high = x1b_high.cvt<float>();

		auto x2r_low  = one - (x1r_low  + x1r_low);
		auto x2r_high = one - (x1r_high + x1r_high);

		x2r_low .store(&X_N2[i * 2 * mipp::nElReg<float>() + 0 * mipp::nElReg<float>()]);
		x2r_high.store(&X_N2[i * 2 * mipp::nElReg<float>() + 1 * mipp::nElReg<float>()]);
	}

	for (unsigned i = vec_loop_size * mipp::nElReg<short>(); i < size; i++)
		X_N2[i] = 1 - (X_N1[i] + X_N1[i]); // (X_N[i] == 1) ? -1 : +1
}

template <>
void Modulator_BPSK_fast<signed char, float>
::modulate(const mipp::vector<signed char>& X_N1, mipp::vector<float>& X_N2) const
{
	auto size = X_N1.size();
	
	const auto vec_loop_size = size / mipp::nElReg<signed char>();
	const mipp::Reg<float> one = 1.f;
	for (unsigned i = 0; i < vec_loop_size; i++)
	{
		auto x1b = mipp::Reg<signed char>(&X_N1[i * mipp::nElReg<signed char>()]);
		auto x1b_low  = x1b.low ().cvt<short>();
		auto x1b_high = x1b.high().cvt<short>();

		auto x1b_low_low   = x1b_low .low ().cvt<int>();
		auto x1b_low_high  = x1b_low .high().cvt<int>();
		auto x1b_high_low  = x1b_high.low() .cvt<int>();
		auto x1b_high_high = x1b_high.high().cvt<int>();

		auto x1r_low_low   = x1b_low_low  .cvt<float>();
		auto x1r_low_high  = x1b_low_high .cvt<float>();
		auto x1r_high_low  = x1b_high_low .cvt<float>();
		auto x1r_high_high = x1b_high_high.cvt<float>();

		auto x2r_low_low   = one - (x1r_low_low   + x1r_low_low  );
		auto x2r_low_high  = one - (x1r_low_high  + x1r_low_high );
		auto x2r_high_low  = one - (x1r_high_low  + x1r_high_low );
		auto x2r_high_high = one - (x1r_high_high + x1r_high_high);

		x2r_low_low  .store(&X_N2[i * 4 * mipp::nElReg<float>() + 0 * mipp::nElReg<float>()]);
		x2r_low_high .store(&X_N2[i * 4 * mipp::nElReg<float>() + 1 * mipp::nElReg<float>()]);
		x2r_high_low .store(&X_N2[i * 4 * mipp::nElReg<float>() + 2 * mipp::nElReg<float>()]);
		x2r_high_high.store(&X_N2[i * 4 * mipp::nElReg<float>() + 3 * mipp::nElReg<float>()]);
	}

	for (unsigned i = vec_loop_size * mipp::nElReg<signed char>(); i < size; i++)
		X_N2[i] = 1 - (X_N1[i] + X_N1[i]); // (X_N[i] == 1) ? -1 : +1
}

template <typename B, typename R>
void Modulator_BPSK_fast<B,R>
::demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const
{
	Y_N2 = Y_N1;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Modulator_BPSK_fast<B_8,R_8>;
template class Modulator_BPSK_fast<B_16,R_16>;
template class Modulator_BPSK_fast<B_32,R_32>;
template class Modulator_BPSK_fast<B_64,R_64>;
#else
template class Modulator_BPSK_fast<B,R>;
#endif
// ==================================================================================== explicit template instantiation