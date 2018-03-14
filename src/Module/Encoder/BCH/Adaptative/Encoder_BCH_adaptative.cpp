#include <vector>
#include <cmath>
#include <iostream>
#include <cmath>

#include "Encoder_BCH_adaptative.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_BCH_adaptative<B>
::Encoder_BCH_adaptative(const int& K, const int& N, const tools::BCH_polynomial_generator& GF_poly, const int n_frames)
 : Encoder_BCH<B>(K, N, GF_poly, n_frames), K_adap(K), N_adap(N)
{
	const std::string name = "Encoder_BCH_adaptative";
	this->set_name(name);

}

template <typename B>
Encoder_BCH_adaptative<B>
::~Encoder_BCH_adaptative()
{
}

template <typename B>
void Encoder_BCH_adaptative<B>
::__encode(const B *U_K, B *bb)
{
}

template <typename B>
void Encoder_BCH_adaptative<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	// generate the parity bits
	this->__encode(U_K, X_N);

	// copy the sys bits at the end of the codeword
	std::copy(U_K, U_K + this->K, X_N + this->N - this->K);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_BCH_adaptative<B_8>;
template class aff3ct::module::Encoder_BCH_adaptative<B_16>;
template class aff3ct::module::Encoder_BCH_adaptative<B_32>;
template class aff3ct::module::Encoder_BCH_adaptative<B_64>;
#else
template class aff3ct::module::Encoder_BCH_adaptative<B>;
#endif
// ==================================================================================== explicit template instantiation
