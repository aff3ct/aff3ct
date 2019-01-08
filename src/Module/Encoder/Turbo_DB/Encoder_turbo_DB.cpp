#include <vector>
#include <cmath>

#include "Tools/Exception/exception.hpp"

#include "Encoder_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_turbo_DB<B>
::Encoder_turbo_DB(const int& K, const int& N, const Interleaver<B> &pi,
                   Encoder_RSC_DB<B> &enco_n, Encoder_RSC_DB<B> &enco_i)
: Encoder<B>(K, N, enco_n.get_n_frames()),
  pi(pi),
  enco_n(enco_n),
  enco_i(enco_i),
  U_K_cpy(K * enco_n.get_n_frames()),
  U_K_i  (K * enco_n.get_n_frames()),
  X_N_tmp(N * enco_n.get_n_frames()),
  par_n(K),
  par_i(K)
{
	const std::string name = "Encoder_turbo_DB";
	this->set_name(name);

	if (K % 2)
	{
		std::stringstream message;
		message << "'K' has to be a divisible by 2 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != 3 * K)
	{
		std::stringstream message;
		message << "'N' has to be equal to 3 * 'K' ('N' = " << N << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() * 2 != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' * 2 has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (!enco_n.is_buffered() || !enco_i.is_buffered())
	{
		std::stringstream message;
		message << "Both 'enco_n.is_buffered()' and 'enco_i.is_buffered()' have to be true ('enco_n.is_buffered()' = "
		        << enco_n.is_buffered() << ", 'enco_i.is_buffered()' = " << enco_i.is_buffered() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enco_n.get_n_frames() != enco_i.get_n_frames())
	{
		std::stringstream message;
		message << "'enco_n.get_n_frames()' has to be equal to 'enco_i.get_n_frames()' ('enco_n.get_n_frames()' = "
		        << enco_n.get_n_frames() << ", 'enco_i.get_n_frames()' = " << enco_i.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

// [   AB   ][  WnWi  ][  YnYi  ]
template <typename B>
void Encoder_turbo_DB<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	std::copy(U_K, U_K + this->K, U_K_cpy.begin() + frame_id * this->K);

	for (auto i = 0; i < this->K; i+=4)
		std::swap(U_K_cpy[frame_id * this->K +i], U_K_cpy[frame_id * this->K + i +1]);

	for (auto i = 0; i < this->K; i += 2)
	{
		const auto l = pi.get_core().get_lut_inv()[i >> 1];
		U_K_i[frame_id * this->K + i +0] = U_K_cpy[frame_id * this->K + l * 2 +0];
		U_K_i[frame_id * this->K + i +1] = U_K_cpy[frame_id * this->K + l * 2 +1];
	}

	enco_n.encode(U_K - frame_id * enco_n.get_K(), X_N_tmp.data(), frame_id);

	std::copy(X_N_tmp.begin() + frame_id * enco_n.get_N() + enco_n.get_K(),
	          X_N_tmp.begin() + frame_id * enco_n.get_N() + enco_n.get_N(),
	          this->par_n.begin());

	enco_i.encode(U_K_i.data(), X_N_tmp.data(), frame_id);

	std::copy(X_N_tmp.begin() + frame_id * enco_i.get_N() + enco_i.get_K(),
	          X_N_tmp.begin() + frame_id * enco_i.get_N() + enco_i.get_N(),
	          this->par_i.begin());

	std::copy(U_K, U_K + this->K, X_N);

	auto j = this->K;
	for (auto i = 0; i < this->K; i+=2) // parity y for both constituent encoders
	{
		X_N[j++] = par_n[i];
		X_N[j++] = par_i[i];
	}

	for (auto i = 1; i < this->K; i+=2) // parity w for both constituent encoders
	{
		X_N[j++] = par_n[i];
		X_N[j++] = par_i[i];
	}
}

template <typename B>
bool Encoder_turbo_DB<B>
::is_codeword(const B *X_N)
{
	auto &U_K_n = X_N_tmp;
	std::copy(X_N, X_N + this->K, U_K_n.begin());

	auto *X_N_par_n = X_N_tmp.data() + this->K;
	for (auto i = 0; i < this->K; i+=2) // parity y for natural encoders
	{
		X_N_par_n[i +0] = X_N[1 * this->K + i +0];
		X_N_par_n[i +1] = X_N[2 * this->K + i +0];
	}

	if (!enco_n.is_codeword(X_N_tmp.data()))
		return false;

	for (auto i = 0; i < this->K; i += 4)
		std::swap(U_K_n[i], U_K_n[i+1]);

	for (auto i = 0; i < this->K; i += 2)
	{
		const auto l = pi.get_core().get_lut_inv()[i >> 1];
		U_K_i[i +0] = U_K_n[l * 2 +0];
		U_K_i[i +1] = U_K_n[l * 2 +1];
	}
	std::copy(U_K_i.begin(), U_K_i.begin() + this->K, X_N_tmp.begin());

	auto *X_N_par_i = X_N_tmp.data() + this->K;
	for (auto i = 0; i < this->K; i+=2) // parity y for interleaver encoders
	{
		X_N_par_i[i +0] = X_N[1 * this->K + i +1];
		X_N_par_i[i +1] = X_N[2 * this->K + i +1];
	}

	if (!enco_i.is_codeword(X_N_tmp.data()))
		return false;

	return true;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_turbo_DB<B_8>;
template class aff3ct::module::Encoder_turbo_DB<B_16>;
template class aff3ct::module::Encoder_turbo_DB<B_32>;
template class aff3ct::module::Encoder_turbo_DB<B_64>;
#else
template class aff3ct::module::Encoder_turbo_DB<B>;
#endif
// ==================================================================================== explicit template instantiation
