#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Quantizer/Pow2/Quantizer_pow2.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R, typename Q>
Quantizer_pow2<R,Q>
::Quantizer_pow2(const int N, const short& fixed_point_pos)
: Quantizer<R,Q>(N),
  val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	const std::string name = "Quantizer_pow2";
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
Quantizer_pow2<float,float>
::Quantizer_pow2(const int N, const short& fixed_point_pos)
: Quantizer<float,float>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_pow2<double,double>
::Quantizer_pow2(const int N, const short& fixed_point_pos)
: Quantizer<double,double>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2";
	this->set_name(name);
}
}
}

template <typename R, typename Q>
Quantizer_pow2<R,Q>
::Quantizer_pow2(const int N, const short& fixed_point_pos, const short& saturation_pos)
: Quantizer<R,Q>(N),
  val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	const std::string name = "Quantizer_pow2";
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
Quantizer_pow2<float, float>
::Quantizer_pow2(const int N, const short& fixed_point_pos, const short& saturation_pos)
: Quantizer<float,float>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_pow2<double, double>
::Quantizer_pow2(const int N, const short& fixed_point_pos, const short& saturation_pos)
: Quantizer<double,double>(N), val_max(0), val_min(0), fixed_point_pos(0), factor(0)
{
	const std::string name = "Quantizer_pow2";
	this->set_name(name);
}
}
}

template<typename R, typename Q>
Quantizer_pow2<R,Q>* Quantizer_pow2<R,Q>
::clone() const
{
	auto m = new Quantizer_pow2(*this);
	m->deep_copy(*this);
	return m;
}

template<typename R, typename Q>
void Quantizer_pow2<R,Q>
::_process(const R *Y_N1, Q *Y_N2, const size_t frame_id)
{
	auto size = (unsigned)(this->N);
	for (unsigned i = 0; i < size; i++)
		Y_N2[i] = (Q)tools::saturate((R)std::round((R)factor * Y_N1[i]), (R)val_min, (R)val_max);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Quantizer_pow2<R_8,Q_8>;
template class aff3ct::module::Quantizer_pow2<R_16,Q_16>;
template class aff3ct::module::Quantizer_pow2<R_32,Q_32>;
template class aff3ct::module::Quantizer_pow2<R_64,Q_64>;
#else
template class aff3ct::module::Quantizer_pow2<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
