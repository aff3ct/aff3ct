#include <sstream>
#include <cmath>
#include <utility>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Quantizer/Pow2/Quantizer_pow2_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R, typename Q>
Quantizer_pow2_fast<R,Q>
::Quantizer_pow2_fast(const int N, const short& fixed_point_pos)
: Quantizer<R,Q>(N),
  val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	const std::string name = "Quantizer_pow2_fast";
	this->set_name(name);

	if (sizeof(Q) * 8 <= (unsigned) fixed_point_pos)
	{
		std::stringstream message;
		message << "'fixed_point_pos' has to be smaller than 'sizeof(Q)' * 8 ('fixed_point_pos' = " << fixed_point_pos
		        << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_pow2_fast<float,float>
::Quantizer_pow2_fast(const int N, const short& fixed_point_pos)
: Quantizer<float,float>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2_fast";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_pow2_fast<double,double>
::Quantizer_pow2_fast(const int N, const short& fixed_point_pos)
: Quantizer<double,double>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2_fast";
	this->set_name(name);
}
}
}

template <typename R, typename Q>
Quantizer_pow2_fast<R,Q>
::Quantizer_pow2_fast(const int N, const short& fixed_point_pos, const short& saturation_pos)
: Quantizer<R,Q>(N),
  val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	const std::string name = "Quantizer_pow2_fast";
	this->set_name(name);

	if (fixed_point_pos <= 0)
	{
		std::stringstream message;
		message << "'fixed_point_pos' has to be greater than 0 ('fixed_point_pos' = " << fixed_point_pos << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (saturation_pos < 2)
	{
		std::stringstream message;
		message << "'saturation_pos' has to be greater than 1 ('saturation_pos' = " << saturation_pos << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (fixed_point_pos > saturation_pos)
	{
		std::stringstream message;
		message << "'saturation_pos' has to be equal or greater than 'fixed_point_pos' ('saturation_pos' = "
		        << saturation_pos << ", 'fixed_point_pos' = " << fixed_point_pos << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (sizeof(Q) * 8 <= (unsigned) fixed_point_pos)
	{
		std::stringstream message;
		message << "'fixed_point_pos' has to be smaller than 'sizeof(Q)' * 8 ('fixed_point_pos' = " << fixed_point_pos
		        << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (val_max > +(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)))
	{
		std::stringstream message;
		message << "'val_max' value is invalid ('val_max' = " << val_max << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (val_min < -(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)))
	{
		std::stringstream message;
		message << "'val_min' value is invalid ('val_min' = " << val_min << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_pow2_fast<float, float>
::Quantizer_pow2_fast(const int N, const short& fixed_point_pos, const short& saturation_pos)
: Quantizer<float,float>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2_fast";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_pow2_fast<double, double>
::Quantizer_pow2_fast(const int N, const short& fixed_point_pos, const short& saturation_pos)
: Quantizer<double,double>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2_fast";
	this->set_name(name);
}
}
}

template<typename R, typename Q>
Quantizer_pow2_fast<R,Q>* Quantizer_pow2_fast<R,Q>
::clone() const
{
	auto m = new Quantizer_pow2_fast(*this);
	m->deep_copy(*this);
	return m;
}

template<typename R, typename Q>
void Quantizer_pow2_fast<R,Q>
::_process(const R *Y_N1, Q *Y_N2, const size_t frame_id)
{
	std::string message = "Supports only 'float' to 'short' and 'float' to 'signed char' conversions.";
	throw tools::runtime_error(__FILE__, __LINE__, __func__, std::move(message));
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_pow2_fast<float,short>
::_process(const float *Y_N1, short *Y_N2, const size_t frame_id)
{
	if (!mipp::isAligned(Y_N1))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N1' is misaligned memory.");

	if (!mipp::isAligned(Y_N2))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N2' is misaligned memory.");

	auto size = (unsigned)(this->N);
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
		Y_N2[i] = (short)tools::saturate((float)std::round((float)factor * Y_N1[i]), (float)val_min, (float)val_max);
}
}
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_pow2_fast<float,signed char>
::_process(const float *Y_N1, signed char *Y_N2, const size_t frame_id)
{
	if (!mipp::isAligned(Y_N1))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N1' is misaligned memory.");

	if (!mipp::isAligned(Y_N2))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'Y_N2' is misaligned memory.");

	auto size = (unsigned)(this->N);
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
		Y_N2[i] = (signed char)tools::saturate((float)std::round((float)factor * Y_N1[i]), (float)val_min, (float)val_max);
}
}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Quantizer_pow2_fast<R_8,Q_8>;
template class aff3ct::module::Quantizer_pow2_fast<R_16,Q_16>;
template class aff3ct::module::Quantizer_pow2_fast<R_32,Q_32>;
#else
template class aff3ct::module::Quantizer_pow2_fast<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
