#include <cassert>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "Quantizer_fast.hpp"

#include "../Tools/bash_tools.h"

template <typename R, typename Q>
Quantizer_fast<R,Q>
::Quantizer_fast(const short& fixed_point_pos)
: val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	assert(sizeof(Q) * 8 > (unsigned) fixed_point_pos);
}

template <>
Quantizer_fast<float,float>
::Quantizer_fast(const short& fixed_point_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <>
Quantizer_fast<double,double>
::Quantizer_fast(const short& fixed_point_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <typename R, typename Q>
Quantizer_fast<R,Q>
::Quantizer_fast(const short& fixed_point_pos, const short& saturation_pos)
: val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	assert(saturation_pos >= 2);
	assert(fixed_point_pos <= saturation_pos);
	assert(sizeof(Q) * 8 > (unsigned) fixed_point_pos);
	assert(val_max <= +(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)));
	assert(val_min >= -(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)));
}

template <>
Quantizer_fast<float, float>
::Quantizer_fast(const short& fixed_point_pos, const short& saturation_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <>
Quantizer_fast<double, double>
::Quantizer_fast(const short& fixed_point_pos, const short& saturation_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}


template <typename R, typename Q>
Quantizer_fast<R,Q>
::~Quantizer_fast()
{
}

template<typename R, typename Q>
void Quantizer_fast<R,Q>
::process(mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2)
{
	std::cerr << bold_red("(EE) Quantizer_fast only support float to short or float to signed char.") << std::endl;
	exit(EXIT_FAILURE);
}

template<>
void Quantizer_fast<float,short>
::process(mipp::vector<float>& Y_N1, mipp::vector<short>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	auto size = Y_N1.size();
	auto vectorized_size = (size / mipp::nElmtsPerRegister<short>()) * mipp::nElmtsPerRegister<short>();
	vectorized_size = (vectorized_size / 2) * 2;

	const auto r_factor = mipp::set1<float>((float) factor);

	for (unsigned i = 0; i < vectorized_size; i += 2 * mipp::nElmtsPerRegister<float>())
	{
		const auto r_Y_N1_0 = mipp::load<float>(&Y_N1[i + 0 * mipp::nElmtsPerRegister<float>()]);
		const auto r_Y_N1_1 = mipp::load<float>(&Y_N1[i + 1 * mipp::nElmtsPerRegister<float>()]);

		auto r_q_0 = mipp::mul<float>(r_factor, r_Y_N1_0);
		auto r_q_1 = mipp::mul<float>(r_factor, r_Y_N1_1);

		r_q_0 = mipp::round<float>(r_q_0);
		r_q_1 = mipp::round<float>(r_q_1);

		r_q_0 = mipp::cvt<float,int>(r_q_0);
		r_q_1 = mipp::cvt<float,int>(r_q_1);

		const auto r_q = mipp::pack<int,short>(r_q_0, r_q_1);

		mipp::store<short>(&Y_N2[i], mipp::sat<short>(r_q, val_min, val_max));
	}

	for (unsigned i = vectorized_size; i < size; i++)
		Y_N2[i] = (short)saturate(std::round(factor * Y_N1[i]));
}

template<>
void Quantizer_fast<float,signed char>
::process(mipp::vector<float>& Y_N1, mipp::vector<signed char>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	auto size = Y_N1.size();
	auto vectorized_size = (size / mipp::nElmtsPerRegister<signed char>()) * mipp::nElmtsPerRegister<signed char>();
	vectorized_size = (vectorized_size / 4) * 4;

	auto r_factor = mipp::set1<float>((float)factor);

	for (unsigned i = 0; i < vectorized_size; i += 4 * mipp::nElmtsPerRegister<float>())
	{
		const auto r_Y_N1_0 = mipp::load<float>(&Y_N1[i + 0 * mipp::nElmtsPerRegister<float>()]);
		const auto r_Y_N1_1 = mipp::load<float>(&Y_N1[i + 1 * mipp::nElmtsPerRegister<float>()]);
		const auto r_Y_N1_2 = mipp::load<float>(&Y_N1[i + 2 * mipp::nElmtsPerRegister<float>()]);
		const auto r_Y_N1_3 = mipp::load<float>(&Y_N1[i + 3 * mipp::nElmtsPerRegister<float>()]);

		auto r_q_0 = mipp::mul<float>(r_factor, r_Y_N1_0);
		auto r_q_1 = mipp::mul<float>(r_factor, r_Y_N1_1);
		auto r_q_2 = mipp::mul<float>(r_factor, r_Y_N1_2);
		auto r_q_3 = mipp::mul<float>(r_factor, r_Y_N1_3);

		r_q_0 = mipp::round<float>(r_q_0);
		r_q_1 = mipp::round<float>(r_q_1);
		r_q_2 = mipp::round<float>(r_q_2);
		r_q_3 = mipp::round<float>(r_q_3);

		r_q_0 = mipp::cvt<float,int>(r_q_0);
		r_q_1 = mipp::cvt<float,int>(r_q_1);
		r_q_2 = mipp::cvt<float,int>(r_q_2);
		r_q_3 = mipp::cvt<float,int>(r_q_3);

		r_q_0 = mipp::pack<int,short>(r_q_0, r_q_1);
		r_q_1 = mipp::pack<int,short>(r_q_2, r_q_3);

		const auto r_q = mipp::pack<short,signed char>(r_q_0, r_q_1);

		mipp::store<signed char>(&Y_N2[i], mipp::sat<signed char>(r_q, val_min, val_max));
	}

	for (unsigned i = vectorized_size; i < size; i++)
		Y_N2[i] = (signed char)saturate(std::round(factor * Y_N1[i]));
}

template <>
void Quantizer_fast<float,float>
::process(mipp::vector<float>& Y_N1, mipp::vector<float>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	std::copy(Y_N1.begin(), Y_N1.end(), Y_N2.begin());
}

template <>
void Quantizer_fast<double,double>
::process(mipp::vector<double>& Y_N1, mipp::vector<double>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	std::copy(Y_N1.begin(), Y_N1.end(), Y_N2.begin());
}

template <typename R, typename Q>
inline R Quantizer_fast<R,Q>
::saturate(R val) const
{
	return std::min(std::max(val, (R)val_min), (R)val_max);
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Quantizer_fast<R_8,Q_8>;
template class Quantizer_fast<R_16,Q_16>;
template class Quantizer_fast<R_32,Q_32>;
#else
template class Quantizer_fast<R,Q>;
#endif
// ==================================================================================== explicit template instantiation