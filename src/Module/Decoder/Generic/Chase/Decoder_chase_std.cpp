#include <chrono>
#include <iostream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common/hard_decide.h"

#include "Module/Decoder/Generic/Chase/Decoder_chase_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_chase_std<B,R>
::Decoder_chase_std(const int K, const int N, const Encoder<B> &encoder, const uint32_t max_flips, const bool hamming)
: Decoder_SIHO<B,R>(K, N),
  encoder(encoder.clone()),
  best_X_N(N),
  less_reliable_llrs(N),
  max_flips(max_flips),
  hamming(hamming),
  min_euclidean_dist(std::numeric_limits<float>::max()),
  min_hamming_dist(std::numeric_limits<uint32_t>::max()),
  best_test(0)
{
	const std::string name = "Decoder_chase_std";
	this->set_name(name);

	if (max_flips > (uint32_t)N)
	{
		std::stringstream message;
		message << "'max_flips' has to be smaller than 'N' ('max_flips' = " << max_flips
		        << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (max_flips > 31)
	{
		std::stringstream message;
		message << "'max_flips' has to be smaller or equal to 31 ('max_flips' = " << max_flips << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_chase_std<B,R>* Decoder_chase_std<B,R>
::clone() const
{
	auto m = new Decoder_chase_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_chase_std<B,R>
::deep_copy(const Decoder_chase_std<B,R> &m)
{
	Module::deep_copy(m);
	if (m.encoder != nullptr) this->encoder.reset(m.encoder->clone());
}

template <typename B, typename R>
int Decoder_chase_std<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
	if (!this->encoder->is_sys())
	{
		std::stringstream message;
		message << "'encoder->is_sys()' has to be true.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto status = this->_decode_siho_cw(Y_N, this->best_X_N.data(), frame_id);

	const auto &info_bits_pos = this->encoder->get_info_bits_pos();
	for (auto k = 0; k < this->K; k++)
		V_K[k] = this->best_X_N[info_bits_pos[k]];

	return status;
}

template <typename B, typename R>
void Decoder_chase_std<B,R>
::_decode_siho_cw_euclidean(const R *Y_N, B *V_N)
{
	uint32_t max = 1 << max_flips;
	for (uint32_t t = 2; t < max; t++)
	{
		// flip the bits and compute the Euclidean dist
		auto cur_euclidean_dist = 0.f;
		for (size_t b = 0; b < this->max_flips; b++)
			if ((t >> b) & 1)
			{
				auto index = this->less_reliable_llrs[b];
				V_N[index] = !V_N[index];
				cur_euclidean_dist += (float)std::abs(Y_N[index]);
			}

		if (cur_euclidean_dist < this->min_euclidean_dist)
			if (encoder->is_codeword(V_N))
			{
				this->min_euclidean_dist = cur_euclidean_dist;
				this->best_test = t;
			}

		// flip back the bits
		for (size_t b = 0; b < this->max_flips; b++)
			if ((t >> b) & 1)
				V_N[this->less_reliable_llrs[b]] = !V_N[this->less_reliable_llrs[b]];
	}

	for (size_t b = 0; b < this->max_flips; b++)
		if ((best_test >> b) & 1)
			V_N[this->less_reliable_llrs[b]] = !V_N[this->less_reliable_llrs[b]];
}

template <typename B, typename R>
void Decoder_chase_std<B,R>
::_decode_siho_cw_hamming(B *V_N)
{
	uint32_t max = 1 << max_flips;
	for (uint32_t t = 2; t < max; t++)
	{
		// flip the bits and compute the Hamming dist
		uint32_t cur_hamming_dist = 0;
		for (size_t b = 0; b < this->max_flips; b++)
			if ((t >> b) & 1)
			{
				auto index = this->less_reliable_llrs[b];
				V_N[index] = !V_N[index];
				cur_hamming_dist++;
			}

		if (cur_hamming_dist < this->min_hamming_dist)
			if (encoder->is_codeword(V_N))
			{
				this->min_hamming_dist = cur_hamming_dist;
				this->best_test = t;
			}

		// flip back the bits
		for (size_t b = 0; b < this->max_flips; b++)
			if ((t >> b) & 1)
				V_N[this->less_reliable_llrs[b]] = !V_N[this->less_reliable_llrs[b]];
	}

	for (size_t b = 0; b < this->max_flips; b++)
		if ((best_test >> b) & 1)
			V_N[this->less_reliable_llrs[b]] = !V_N[this->less_reliable_llrs[b]];
}

template <typename B, typename R>
int Decoder_chase_std<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
	tools::hard_decide(Y_N, V_N, this->N);

	if (this->max_flips && !this->encoder->is_codeword(V_N))
	{
		std::iota(less_reliable_llrs.begin(), less_reliable_llrs.end(), 0);

		std::partial_sort(less_reliable_llrs.begin(),
		                  less_reliable_llrs.begin() + this->max_flips,
		                  less_reliable_llrs.end(),
		                  [&Y_N](const uint32_t i1, const uint32_t i2) {
			return std::abs(Y_N[i1]) < std::abs(Y_N[i2]);
		});

		V_N[less_reliable_llrs[0]] = !V_N[less_reliable_llrs[0]];
		if (!this->encoder->is_codeword(V_N))
		{
			V_N[less_reliable_llrs[0]] = !V_N[less_reliable_llrs[0]];
			if (this->hamming)
			{
				this->min_hamming_dist = std::numeric_limits<uint32_t>::max();
				this->_decode_siho_cw_hamming(V_N);
			}
			else
			{
				this->min_euclidean_dist = std::numeric_limits<float>::max();
				this->_decode_siho_cw_euclidean(Y_N, V_N);
			}
		}
	}

	return 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_chase_std<B_8,Q_8>;
template class aff3ct::module::Decoder_chase_std<B_16,Q_16>;
template class aff3ct::module::Decoder_chase_std<B_32,Q_32>;
template class aff3ct::module::Decoder_chase_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_chase_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation