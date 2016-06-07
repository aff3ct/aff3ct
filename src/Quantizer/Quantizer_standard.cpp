#include <cassert>
#include <algorithm>
#include <cmath>

#include "Quantizer_standard.hpp"

template <typename R, typename Q>
Quantizer_standard<R,Q>
::Quantizer_standard(const short& fixed_point_pos)
: val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	assert(sizeof(Q) * 8 > (unsigned) fixed_point_pos);
}

template <>
Quantizer_standard<float,float>
::Quantizer_standard(const short& fixed_point_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <>
Quantizer_standard<double,double>
::Quantizer_standard(const short& fixed_point_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <typename R, typename Q>
Quantizer_standard<R,Q>
::Quantizer_standard(const short& fixed_point_pos, const short& saturation_pos)
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
Quantizer_standard<float, float>
::Quantizer_standard(const short& fixed_point_pos, const short& saturation_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <>
Quantizer_standard<double, double>
::Quantizer_standard(const short& fixed_point_pos, const short& saturation_pos)
: val_max(0),
  val_min(0),
  fixed_point_pos(0),
  factor(0)
{
}

template <typename R, typename Q>
Quantizer_standard<R,Q>
::~Quantizer_standard()
{
}

template<typename R, typename Q>
void Quantizer_standard<R,Q>
::process(mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	auto size = Y_N1.size();
	for (unsigned i = 0; i < size; i++)
		Y_N2[i] = (Q)saturate(round((R)factor * Y_N1[i]));
}

template <>
void Quantizer_standard<float,float>
::process(mipp::vector<float>& Y_N1, mipp::vector<float>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	for (unsigned i = 0; i < Y_N1.size(); i++)
		Y_N2[i] = Y_N1[i];
}

template <>
void Quantizer_standard<double,float>
::process(mipp::vector<double>& Y_N1, mipp::vector<float>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	for (unsigned i = 0; i < Y_N1.size(); i++)
		Y_N2[i] = (float)Y_N1[i];
}

template <>
void Quantizer_standard<float,double>
::process(mipp::vector<float>& Y_N1, mipp::vector<double>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	for (unsigned i = 0; i < Y_N1.size(); i++)
		Y_N2[i] = (double)Y_N1[i];
}

template <>
void Quantizer_standard<double,double>
::process(mipp::vector<double>& Y_N1, mipp::vector<double>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	for (unsigned i = 0; i < Y_N1.size(); i++)
		Y_N2[i] = Y_N1[i];
}

template <typename R, typename Q>
inline R Quantizer_standard<R,Q>
::saturate(R val) const
{
	return std::min(std::max(val, (R)val_min), (R)val_max);
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Quantizer_standard<R_8,Q_8>;
template class Quantizer_standard<R_16,Q_16>;
template class Quantizer_standard<R_32,Q_32>;
template class Quantizer_standard<R_64,Q_64>;
#else
template class Quantizer_standard<R,Q>;
#endif
// ==================================================================================== explicit template instantiation