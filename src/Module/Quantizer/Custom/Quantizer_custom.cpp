#include <algorithm>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Quantizer/Custom/Quantizer_custom.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R, typename Q>
Quantizer_custom<R,Q>
::Quantizer_custom(const int N)
: Quantizer<R,Q>(N),
  val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  delta_inv((R)0)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_custom<float,float>
::Quantizer_custom(const int N)
: Quantizer<float,float>(N), val_max(0), val_min(0), delta_inv(0.f)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_custom<double,double>
::Quantizer_custom(const int N)
: Quantizer<double,double>(N), val_max(0), val_min(0), delta_inv(0.f)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}
}
}

template <typename R, typename Q>
Quantizer_custom<R,Q>
::Quantizer_custom(const int N, const short& saturation_pos)
: Quantizer<R,Q>(N),
  val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
  val_min(-val_max),
  delta_inv((R)0)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);

	if (sizeof(Q) * 8 < (unsigned) saturation_pos)
	{
		std::stringstream message;
		message << "'saturation_pos' has to be equal or smaller than 'sizeof(Q)' * 8 ('saturation_pos' = "
		        << saturation_pos << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_custom<float,float>
::Quantizer_custom(const int N, const short& saturation_pos)
: Quantizer<float,float>(N), val_max(0), val_min(0), delta_inv(0.f)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_custom<double,double>
::Quantizer_custom(const int N, const short& saturation_pos)
: Quantizer<double,double>(N), val_max(0), val_min(0), delta_inv(0.f)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}
}
}

template <typename R, typename Q>
Quantizer_custom<R,Q>
::Quantizer_custom(const int N, const float min_max)
: Quantizer<R,Q>(N),
  val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  delta_inv((R)1.0 / ((R)std::abs(min_max) / (R)val_max))
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_custom<float,float>
::Quantizer_custom(const int N, const float min_max)
: Quantizer<float,float>(N), val_max(0), val_min(0), delta_inv(0.f)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_custom<double,double>
::Quantizer_custom(const int N, const float min_max)
: Quantizer<double,double>(N), val_max(0), val_min(0), delta_inv(0.f)
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);
}
}
}

template <typename R, typename Q>
Quantizer_custom<R,Q>
::Quantizer_custom(const int N, const float min_max, const short& saturation_pos)
: Quantizer<R,Q>(N),
  val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
  val_min(-val_max),
  delta_inv((R)1.0 / ((R)std::abs(min_max) / (R)val_max))
{
	const std::string name = "Quantizer_custom";
	this->set_name(name);

	if (sizeof(Q) * 8 < (unsigned) saturation_pos)
	{
		std::stringstream message;
		message << "'saturation_pos' has to be equal or smaller than 'sizeof(Q)' * 8 ('saturation_pos' = "
		        << saturation_pos << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename R, typename Q>
Quantizer_custom<R,Q>* Quantizer_custom<R,Q>
::clone() const
{
	auto m = new Quantizer_custom(*this);
	m->deep_copy(*this);
	return m;
}

template<typename R, typename Q>
void Quantizer_custom<R,Q>
::_process(const R *Y_N1, Q *Y_N2, const size_t frame_id)
{
	const auto size = (unsigned)(this->N);

	if (delta_inv == (R)0)
	{
		std::vector<R> tmp(size);
		R avg = 0;
		for (unsigned i = 0; i < size; i++)
		{
			tmp[i] = std::abs(Y_N1[i]);
			avg += tmp[i];
		}
		avg /= tmp.size();
		std::sort(tmp.begin(), tmp.end());

		delta_inv = (R)1.0 / ((R)std::abs(tmp[(tmp.size() / 10) * 8]) / (R)val_max);
	}

	for (unsigned i = 0; i < size; i++)
		Y_N2[i] = (Q)tools::saturate(std::round(Y_N1[i] * delta_inv), (R)val_min, (R)val_max);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Quantizer_custom<R_8,Q_8>;
template class aff3ct::module::Quantizer_custom<R_16,Q_16>;
template class aff3ct::module::Quantizer_custom<R_32,Q_32>;
template class aff3ct::module::Quantizer_custom<R_64,Q_64>;
#else
template class aff3ct::module::Quantizer_custom<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
