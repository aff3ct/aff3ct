#include <algorithm>
#include <cmath>

#include "Quantizer_NO.hpp"

using namespace aff3ct::module;

template <typename R, typename Q>
Quantizer_NO<R,Q>
::Quantizer_NO(const int N, const int n_frames)
: Quantizer<R,Q>(N, n_frames)
{
	const std::string name = "Quantizer_NO";
	this->set_name(name);
}

template <typename R, typename Q>
Quantizer_NO<R,Q>
::~Quantizer_NO()
{
}

template<typename R, typename Q>
void Quantizer_NO<R,Q>
::process(const R *Y_N1, Q *Y_N2)
{
	const auto loop_size = (unsigned)(this->N * this->n_frames);
	for (unsigned i = 0; i < loop_size; i++)
		Y_N2[i] = (Q)Y_N1[i];
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_NO<float,float>
::process(const float *Y_N1, float *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N * this->n_frames, Y_N2);
}
}
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_NO<double,double>
::process(const double *Y_N1, double *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N * this->n_frames, Y_N2);
}
}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Quantizer_NO<R_8,Q_8>;
template class aff3ct::module::Quantizer_NO<R_16,Q_16>;
template class aff3ct::module::Quantizer_NO<R_32,Q_32>;
template class aff3ct::module::Quantizer_NO<R_64,Q_64>;
#else
template class aff3ct::module::Quantizer_NO<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
