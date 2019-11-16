#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_RA<B>
::Encoder_RA(const int& K, const int& N, const Interleaver<B>& interleaver, const int n_frames)
 : Encoder<B>(K, N, n_frames), rep_count(N/K), U(N), tmp_X_N(N), interleaver(interleaver)
{
	const std::string name = "Encoder_RA";
	this->set_name(name);
	this->set_sys(false);

	if (N % K)
	{
		std::stringstream message;
		message << "'K' has to be a multiple of 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)interleaver.get_core().get_size() != N)
	{
		std::stringstream message;
		message << "'interleaver.get_core().get_size()' has to be equal to 'N' ('interleaver.get_core().get_size()' = "
		        << interleaver.get_core().get_size() << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto k = 0; k < this->K; k++)
		this->info_bits_pos[k] = rep_count * k;
}

template <typename B>
Encoder_RA<B>* Encoder_RA<B>
::clone() const
{
	auto m = new Encoder_RA<B>(*this); // soft copy constructor
	m->deep_copy(*this); // hard copy
	return m;
}

template <typename B>
void Encoder_RA<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	// repetition
	for (auto i = 0; i < this->K; i++)
		for (auto j = 0; j < rep_count; j++)
			U[i * rep_count +j] = U_K[i];

	interleaver.interleave(U.data(), tmp_X_N.data(), frame_id);

	// accumulation
	for (auto i = 1; i < this->N; i++)
		tmp_X_N[i] = tmp_X_N[i-1] ^ tmp_X_N[i];

	std::copy(tmp_X_N.begin(), tmp_X_N.end(), X_N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_RA<B_8>;
template class aff3ct::module::Encoder_RA<B_16>;
template class aff3ct::module::Encoder_RA<B_32>;
template class aff3ct::module::Encoder_RA<B_64>;
#else
template class aff3ct::module::Encoder_RA<B>;
#endif
// ==================================================================================== explicit template instantiation
