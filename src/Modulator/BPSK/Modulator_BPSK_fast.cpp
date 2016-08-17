#include "../../Tools/bash_tools.h"
#include "Modulator_BPSK_fast.hpp"

template <typename B, typename R>
Modulator_BPSK_fast<B,R>
::Modulator_BPSK_fast(const R sigma, const std::string name)
: Modulator<B,R>(name), two_on_square_sigma((R)2.0 / (sigma * sigma))
{
}

template <typename B, typename R, typename Q>
Modulator_BPSK_fast<B,R,Q>
::~Modulator_BPSK_fast()
{
}

template <typename B, typename R, typename Q>
void Modulator_BPSK_fast<B,R,Q>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	std::cerr << bold_red("(EE) The fast modulator does not support this type of data.") << std::endl;
	std::exit(-1);
}

template <>
void Modulator_BPSK_fast<int, float, float>
::modulate(const mipp::vector<int>& X_N1, mipp::vector<float>& X_N2)
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
void Modulator_BPSK_fast<short, float, float>
::modulate(const mipp::vector<short>& X_N1, mipp::vector<float>& X_N2)
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
void Modulator_BPSK_fast<signed char, float, float>
::modulate(const mipp::vector<signed char>& X_N1, mipp::vector<float>& X_N2)
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
		auto x1b_high_low  = x1b_high.low ().cvt<int>();
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

template <typename B, typename R, typename Q>
void Modulator_BPSK_fast<B,R,Q>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	if (disable_sig2)
		Y_N2 = Y_N1;
	else
	{
		auto size = Y_N1.size();
		auto vec_loop_size = (size / mipp::nElReg<Q>()) * mipp::nElReg<Q>();
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			auto y = mipp::Reg<Q>(&Y_N1[i]) * two_on_square_sigma;
			y.store(&Y_N2[i]);
		}
		for (unsigned i = vec_loop_size; i < size; i++)
			Y_N2[i] = Y_N1[i] * two_on_square_sigma;
	}
}

template <typename B, typename R, typename Q>
void Modulator_BPSK_fast<B,R,Q>
::demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	auto size = Y_N1.size();
	if (disable_sig2)
	{
		auto vec_loop_size = (size / mipp::nElReg<Q>()) * mipp::nElReg<Q>();
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			auto y = mipp::add(mipp::Reg<Q>(&Y_N1[i]), mipp::Reg<Q>(&Y_N2[i]));
			y.store(&Y_N3[i]);
		}
		for (unsigned i = vec_loop_size; i < size; i++)
			Y_N3[i] = Y_N1[i] + Y_N2[i];
	}
	else
	{
		auto vec_loop_size = (size / mipp::nElReg<Q>()) * mipp::nElReg<Q>();
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			// mipp::fmadd(a, b, c) = a * b + c
			auto y = mipp::fmadd(mipp::Reg<Q>(&Y_N1[i]), mipp::Reg<Q>(two_on_square_sigma), mipp::Reg<Q>(&Y_N2[i]));
			y.store(&Y_N3[i]);
		}
		for (unsigned i = vec_loop_size; i < size; i++)
			Y_N3[i] = (Y_N1[i] * two_on_square_sigma) + Y_N2[i];
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template struct Modulator_BPSK_fast<B_8,R_8,R_8>;
template struct Modulator_BPSK_fast<B_8,R_8,Q_8>;
template struct Modulator_BPSK_fast<B_16,R_16,R_16>;
template struct Modulator_BPSK_fast<B_16,R_16,Q_16>;
template struct Modulator_BPSK_fast<B_32,R_32,R_32>;
template struct Modulator_BPSK_fast<B_64,R_64,R_64>;
#else
template struct Modulator_BPSK_fast<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template struct Modulator_BPSK_fast<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation