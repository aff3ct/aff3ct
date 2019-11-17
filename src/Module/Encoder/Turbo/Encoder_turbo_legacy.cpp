#include <string>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_turbo_legacy<B>
::Encoder_turbo_legacy(const int& K, const int& N, const Interleaver<B> &pi, const Encoder<B> &sub_enc)
: Encoder_turbo<B>(K, N, pi, sub_enc, sub_enc),
  pi(pi),
  sub_enc(sub_enc.clone()),
  X_N_n((2 * (K + sub_enc.tail_length()/2))*sub_enc.get_n_frames()),
  X_N_i((2 * (K + sub_enc.tail_length()/2))*sub_enc.get_n_frames())
{
	const std::string name = "Encoder_turbo_legacy";
	this->set_name(name);

	if (N - 2 * sub_enc.tail_length() != 3 * K)
	{
		std::stringstream message;
		message << "'N' - 2 * 'sub_enc.tail_length()' has to be equal to 3 * 'K' ('N' = " << N
		        << ", 'sub_enc.tail_length()' = " << sub_enc.tail_length() << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto k = 0; k < this->K; k++)
		this->info_bits_pos[k] = 3 * k;
}

template <typename B>
Encoder_turbo_legacy<B>* Encoder_turbo_legacy<B>
::clone() const
{
	auto m = new Encoder_turbo_legacy(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_turbo_legacy<B>
::deep_copy(const Encoder_turbo_legacy<B> &m)
{
	Encoder_turbo<B>::deep_copy(m);
	this->sub_enc.reset(m.sub_enc->clone());
}

template <typename B>
void Encoder_turbo_legacy<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	pi.interleave  (U_K - frame_id * this->K, this->U_K_i.data(), frame_id);
	sub_enc->encode(U_K - frame_id * this->K,       X_N_n.data(), frame_id);
	sub_enc->encode(this->U_K_i.data(),             X_N_i.data(), frame_id);

	const auto off1 = ((3*this->K + (2 * sub_enc->tail_length())) * frame_id) - (frame_id * this->N);
	const auto off2 =  (2*this->K + (1 * sub_enc->tail_length())) * frame_id;
	for (auto i = 0; i < this->K; i++)
	{
		X_N[off1 + 3*i +0] = X_N_n[off2 + 2*i +0];
		X_N[off1 + 3*i +1] = X_N_n[off2 + 2*i +1];
		X_N[off1 + 3*i +2] = X_N_i[off2 + 2*i +1];
	}

	const auto off1_tails_n = off1 + 3*this->K;
	const auto off2_tails_n = off2 + 2*this->K;
	for (auto i = 0; i < sub_enc->tail_length() / 2; i++)
	{
		X_N[off1_tails_n + 2*i +0] = X_N_n[off2_tails_n + 2*i +0];
		X_N[off1_tails_n + 2*i +1] = X_N_n[off2_tails_n + 2*i +1];
	}

	const auto off1_tails_i = off1_tails_n + sub_enc->tail_length();
	const auto off2_tails_i = off2_tails_n;
	for (auto i = 0; i < sub_enc->tail_length() / 2; i++)
	{
		X_N[off1_tails_i + 2*i +0] = X_N_i[off2_tails_i + 2*i +0];
		X_N[off1_tails_i + 2*i +1] = X_N_i[off2_tails_i + 2*i +1];
	}
}

template <typename B>
bool Encoder_turbo_legacy<B>
::is_codeword(const B *X_N)
{
	auto &U_K_n = this->X_N_tmp;

	for (auto i = 0; i < this->K; i++)
	{
		U_K_n[i] = X_N_n[2*i +0] = X_N[3*i +0];
		           X_N_n[2*i +1] = X_N[3*i +1];
		           X_N_i[2*i +1] = X_N[3*i +2];
	}

	const auto off1_tails_n = 3*this->K;
	const auto off2_tails_n = 2*this->K;
	for (auto i = 0; i < sub_enc->tail_length() / 2; i++)
	{
		X_N_n[off2_tails_n + 2*i +0] = X_N[off1_tails_n + 2*i +0];
		X_N_n[off2_tails_n + 2*i +1] = X_N[off1_tails_n + 2*i +1];
	}

	const auto off1_tails_i = off1_tails_n + sub_enc->tail_length();
	const auto off2_tails_i = off2_tails_n;
	for (auto i = 0; i < sub_enc->tail_length() / 2; i++)
	{
		X_N_i[off2_tails_i + 2*i +0] = X_N[off1_tails_i + 2*i +0];
		X_N_i[off2_tails_i + 2*i +1] = X_N[off1_tails_i + 2*i +1];
	}

	if (!sub_enc->is_codeword(X_N_n.data()))
		return false;

	pi.interleave(U_K_n.data(), this->U_K_i.data());

	for (auto i = 0; i < this->K; i++)
		X_N_i[2*i +0] = this->U_K_i[i];

	if (!sub_enc->is_codeword(X_N_i.data()))
		return false;

	return true;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_turbo_legacy<B_8>;
template class aff3ct::module::Encoder_turbo_legacy<B_16>;
template class aff3ct::module::Encoder_turbo_legacy<B_32>;
template class aff3ct::module::Encoder_turbo_legacy<B_64>;
#else
template class aff3ct::module::Encoder_turbo_legacy<B>;
#endif
// ==================================================================================== explicit template instantiation
