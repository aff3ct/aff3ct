#include <cassert>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "Tools/Math/utils.h"
#include "Tools/Display/bash_tools.h"

#include "Quantizer_fast.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R, typename Q>
Quantizer_fast<R,Q>
::Quantizer_fast(const int N, const short& fixed_point_pos, const int n_frames, const std::string name)
: Quantizer<R,Q>(N, n_frames, name),
  val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	assert(sizeof(Q) * 8 > (unsigned) fixed_point_pos);
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_fast<float,float>
::Quantizer_fast(const int N, const short& fixed_point_pos, const int n_frames, const std::string name)
: Quantizer<float,float>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_fast<double,double>
::Quantizer_fast(const int N, const short& fixed_point_pos, const int n_frames, const std::string name)
: Quantizer<double,double>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

template <typename R, typename Q>
Quantizer_fast<R,Q>
::Quantizer_fast(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames, 
                 const std::string name)
: Quantizer<R,Q>(N, n_frames, name),
  val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
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

namespace aff3ct
{
namespace module
{
template <>
Quantizer_fast<float, float>
::Quantizer_fast(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames, 
                 const std::string name)
: Quantizer<float,float>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_fast<double, double>
::Quantizer_fast(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames, 
                 const std::string name)
: Quantizer<double,double>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

template <typename R, typename Q>
Quantizer_fast<R,Q>
::~Quantizer_fast()
{
}

template<typename R, typename Q>
void Quantizer_fast<R,Q>
::process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2)
{
	std::cerr << bold_red("(EE) Quantizer_fast only support float to short or float to signed char.") << std::endl;
	exit(EXIT_FAILURE);
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_fast<float,short>
::process(const mipp::vector<float>& Y_N1, mipp::vector<short>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	auto size = (unsigned)Y_N1.size();
	auto vectorized_size = (size / mipp::nElmtsPerRegister<short>()) * mipp::nElmtsPerRegister<short>();
	vectorized_size = (vectorized_size / 2) * 2;

	const auto r_factor = mipp::Reg<float>((float) factor);

	for (unsigned i = 0; i < vectorized_size; i += 2 * mipp::nElmtsPerRegister<float>())
	{
		const auto r_q32_0 = r_factor * &Y_N1[i + 0 * mipp::nElmtsPerRegister<float>()];
		const auto r_q32_1 = r_factor * &Y_N1[i + 1 * mipp::nElmtsPerRegister<float>()];

		const auto r_q32i_0 = r_q32_0.round().cvt<int>();
		const auto r_q32i_1 = r_q32_1.round().cvt<int>();

		const auto r_q16i = mipp::pack<int,short>(r_q32i_0, r_q32i_1);
		r_q16i.sat(val_min, val_max).store(&Y_N2[i]);
	}

	for (unsigned i = vectorized_size; i < size; i++)
		Y_N2[i] = (short)saturate((float)std::round((float)factor * Y_N1[i]), (float)val_min, (float)val_max);
}
}
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_fast<float,signed char>
::process(const mipp::vector<float>& Y_N1, mipp::vector<signed char>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	auto size = (unsigned)Y_N1.size();
	auto vectorized_size = (size / mipp::nElmtsPerRegister<signed char>()) * mipp::nElmtsPerRegister<signed char>();
	vectorized_size = (vectorized_size / 4) * 4;

	auto r_factor = mipp::Reg<float>((float)factor);

	for (unsigned i = 0; i < vectorized_size; i += 4 * mipp::nElmtsPerRegister<float>())
	{
		const auto r_q32_0 = r_factor * &Y_N1[i + 0 * mipp::nElmtsPerRegister<float>()];
		const auto r_q32_1 = r_factor * &Y_N1[i + 1 * mipp::nElmtsPerRegister<float>()];
		const auto r_q32_2 = r_factor * &Y_N1[i + 2 * mipp::nElmtsPerRegister<float>()];
		const auto r_q32_3 = r_factor * &Y_N1[i + 3 * mipp::nElmtsPerRegister<float>()];

		const auto r_q32i_0 = r_q32_0.round().cvt<int>();
		const auto r_q32i_1 = r_q32_1.round().cvt<int>();
		const auto r_q32i_2 = r_q32_2.round().cvt<int>();
		const auto r_q32i_3 = r_q32_3.round().cvt<int>();

		const auto r_q16i_0 = mipp::pack<int,short>(r_q32i_0, r_q32i_1);
		const auto r_q16i_1 = mipp::pack<int,short>(r_q32i_2, r_q32i_3);

		const auto r_q8i = mipp::pack<short,signed char>(r_q16i_0, r_q16i_1);
		r_q8i.sat(val_min, val_max).store(&Y_N2[i]);
	}

	for (unsigned i = vectorized_size; i < size; i++)
		Y_N2[i] = (signed char)saturate((float)std::round((float)factor * Y_N1[i]), (float)val_min, (float)val_max);
}
}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Quantizer_fast<R_8,Q_8>;
template class aff3ct::module::Quantizer_fast<R_16,Q_16>;
template class aff3ct::module::Quantizer_fast<R_32,Q_32>;
#else
template class aff3ct::module::Quantizer_fast<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
