#include <cassert>
#include <algorithm>
#include <cmath>

#include "Quantizer_NO.hpp"

using namespace aff3ct::module;

template <typename R, typename Q>
Quantizer_NO<R,Q>
::Quantizer_NO(const int N, const int n_frames, const std::string name)
: Quantizer<R,Q>(N, n_frames, name)
{
}

template <typename R, typename Q>
Quantizer_NO<R,Q>
::~Quantizer_NO()
{
}

template<typename R, typename Q>
void Quantizer_NO<R,Q>
::process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	const auto loop_size = Y_N1.size();
	for (unsigned i = 0; i < loop_size; i++)
		Y_N2[i] = (Q)Y_N1[i];
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_NO<float,float>
::process(const mipp::vector<float>& Y_N1, mipp::vector<float>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	Y_N2 = Y_N1;	
}
}
}

namespace aff3ct
{
namespace module
{
template<>
void Quantizer_NO<double,double>
::process(const mipp::vector<double>& Y_N1, mipp::vector<double>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());
	Y_N2 = Y_N1;	
}
}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Quantizer_NO<R_8,Q_8>;
template class Quantizer_NO<R_16,Q_16>;
template class Quantizer_NO<R_32,Q_32>;
template class Quantizer_NO<R_64,Q_64>;
#else
template class Quantizer_NO<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
