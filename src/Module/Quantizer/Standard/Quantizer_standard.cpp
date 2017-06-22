#include <algorithm>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Quantizer_standard.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R, typename Q>
Quantizer_standard<R,Q>
::Quantizer_standard(const int N, const short& fixed_point_pos, const int n_frames, const std::string name)
: Quantizer<R,Q>(N, n_frames, name), 
  val_max(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	if (sizeof(Q) * 8 <= (unsigned) fixed_point_pos)
	{
		std::stringstream message;
		message << "'fixed_point_pos' has to be smaller than 'sizeof(Q)' * 8 ('fixed_point_pos' = " << fixed_point_pos
		        << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_standard<float,float>
::Quantizer_standard(const int N, const short& fixed_point_pos, const int n_frames, const std::string name)
: Quantizer<float,float>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_standard<double,double>
::Quantizer_standard(const int N, const short& fixed_point_pos, const int n_frames, const std::string name)
: Quantizer<double,double>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

template <typename R, typename Q>
Quantizer_standard<R,Q>
::Quantizer_standard(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames, 
                     const std::string name)
: Quantizer<R,Q>(N, n_frames, name), 
  val_max(((1 << (saturation_pos -2))) + ((1 << (saturation_pos -2)) -1)),
  val_min(-val_max),
  fixed_point_pos(fixed_point_pos),
  factor(1 << fixed_point_pos)
{
	if (fixed_point_pos <= 0)
	{
		std::stringstream message;
		message << "'fixed_point_pos' has to be greater than 0 ('fixed_point_pos' = " << fixed_point_pos << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (saturation_pos < 2)
	{
		std::stringstream message;
		message << "'saturation_pos' has to be greater than 1 ('saturation_pos' = " << saturation_pos << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (fixed_point_pos > saturation_pos)
	{
		std::stringstream message;
		message << "'saturation_pos' has to be equal or greater than 'fixed_point_pos' ('saturation_pos' = "
		        << saturation_pos << ", 'fixed_point_pos' = " << fixed_point_pos << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (sizeof(Q) * 8 <= (unsigned) fixed_point_pos)
	{
		std::stringstream message;
		message << "'fixed_point_pos' has to be smaller than 'sizeof(Q)' * 8 ('fixed_point_pos' = " << fixed_point_pos
		        << ", 'sizeof(Q)' = " << sizeof(Q) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (val_max > +(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)))
	{
		std::stringstream message;
		message << "'val_max' value is invalid ('val_max' = " << val_max << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (val_min < -(((1 << ((sizeof(Q) * 8) -2))) + ((1 << ((sizeof(Q) * 8) -2)) -1)))
	{
		std::stringstream message;
		message << "'val_min' value is invalid ('val_min' = " << val_min << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_standard<float, float>
::Quantizer_standard(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames, 
                     const std::string name)
: Quantizer<float,float>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

namespace aff3ct
{
namespace module
{
template <>
Quantizer_standard<double, double>
::Quantizer_standard(const int N, const short& fixed_point_pos, const short& saturation_pos, const int n_frames, 
                     const std::string name)
: Quantizer<double,double>(N, n_frames, name), val_max(0), val_min(0), fixed_point_pos(0), factor(0) {}
}
}

template <typename R, typename Q>
Quantizer_standard<R,Q>
::~Quantizer_standard()
{
}

template<typename R, typename Q>
void Quantizer_standard<R,Q>
::process(const R *Y_N1, Q *Y_N2)
{
	auto size = (unsigned)(this->N * this->n_frames);
	for (unsigned i = 0; i < size; i++)
		Y_N2[i] = (Q)saturate((R)std::round((R)factor * Y_N1[i]), (R)val_min, (R)val_max);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Quantizer_standard<R_8,Q_8>;
template class aff3ct::module::Quantizer_standard<R_16,Q_16>;
template class aff3ct::module::Quantizer_standard<R_32,Q_32>;
template class aff3ct::module::Quantizer_standard<R_64,Q_64>;
#else
template class aff3ct::module::Quantizer_standard<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
