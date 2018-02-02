#include <chrono>
#include <iostream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Perf/hard_decision.h"

#include "Decoder_chase_naive.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_chase_naive<B,R>
::Decoder_chase_naive(const int K, const int N, Encoder<B> &encoder, const uint32_t max_flips, const bool hamming, 
                      const int n_frames)
: Decoder          (K, N, n_frames, 1),
  Decoder_SIHO<B,R>(K, N, n_frames, 1),
  encoder(encoder),
  X_N(N+7), // +7 to avoid segmentation fault when casting B to uint64_t
  best_X_N(N),
  hard_Y_N(N),
  max_flips(max_flips),
  hamming(hamming),
  min_euclidean_dist(std::numeric_limits<float>::max()),
  min_hamming_dist(std::numeric_limits<uint32_t>::max()),
  llrs_indexes(N),
  flip_pos(max_flips)
{
	const std::string name = "Decoder_chase_naive";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_chase_naive<B,R>
::~Decoder_chase_naive()
{
}

template <typename B, typename R>
void Decoder_chase_naive<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	if (!this->encoder.is_sys())
	{
		std::stringstream message;
		message << "'encoder.is_sys()' has to be true.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->_decode_siho_cw(Y_N, this->best_X_N.data(), frame_id);

	const auto &info_bits_pos = this->encoder.get_info_bits_pos();
	for (auto k = 0; k < this->K; k++)
		V_K[k] = this->best_X_N[info_bits_pos[k]];
}

template <typename B, typename R>
void Decoder_chase_naive<B,R>
::_decode_siho_cw_recursive(const R *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips)
{
	if (n_flips == max_flips)
	{
		std::copy(this->hard_Y_N.begin(), this->hard_Y_N.end(), this->X_N.begin());

		// flip the bits
		for (uint32_t f = 0; f < max_flips; f++)
			this->X_N[this->llrs_indexes[this->flip_pos[f]]] = !this->X_N[this->llrs_indexes[this->flip_pos[f]]];

		if (this->encoder.is_codeword(this->X_N.data()))
		{
			// compute the Euclidean dist
			auto cur_euclidean_dist = 0.f;
			for (uint32_t f = 0; f < max_flips; f++)
				cur_euclidean_dist += std::abs(Y_N[this->llrs_indexes[this->flip_pos[f]]]);

			// update the best codeword
			if (cur_euclidean_dist < this->min_euclidean_dist)
			{
				this->min_euclidean_dist = cur_euclidean_dist;
				std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);
			}
		}
	}
	else
	{
		for (auto flip_pos = n_flips; flip_pos < (uint32_t)this->N; flip_pos++)
		{
			this->flip_pos[n_flips] = flip_pos;
			this->_decode_siho_cw_recursive(Y_N, V_N, n_flips +1, max_flips);
		}
	}
}

template <typename B, typename R>
bool Decoder_chase_naive<B,R>
::_decode_siho_cw_recursive_hard(const B *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips)
{
	if (n_flips == max_flips)
	{
		std::copy(Y_N, Y_N + this->N, this->X_N.begin());

		// flip the bits
		for (uint32_t f = 0; f < max_flips; f++)
			this->X_N[this->llrs_indexes[this->flip_pos[f]]] = !this->X_N[this->llrs_indexes[this->flip_pos[f]]];

		if (this->encoder.is_codeword(this->X_N.data()))
		{
			// update the best codeword
			if (max_flips < this->min_hamming_dist)
			{
				this->min_hamming_dist = max_flips;
				std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);

				return true;
			}
		}
	}
	else
	{
		for (auto flip_pos = n_flips; flip_pos < (uint32_t)this->N; flip_pos++)
		{
			this->flip_pos[n_flips] = flip_pos;
			if (this->_decode_siho_cw_recursive_hard(Y_N, V_N, n_flips +1, max_flips))
				return true;
		}
	}

	return false;
}

template <typename B, typename R>
void Decoder_chase_naive<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	tools::hard_decide(Y_N, this->hard_Y_N.data(), this->N);

	if (this->encoder.is_codeword(this->hard_Y_N.data()))
	{
		std::copy(this->hard_Y_N.begin(), this->hard_Y_N.begin() + this->N, V_N);
	}
	else
	{
		std::iota(llrs_indexes.begin(), llrs_indexes.end(), 0);
			std::partial_sort(llrs_indexes.begin(), llrs_indexes.begin() + this->max_flips, llrs_indexes.end(),
			[&Y_N](const uint32_t i1, const uint32_t i2) { 
				return std::abs(Y_N[i1]) < std::abs(Y_N[i2]);
			});

		if (this->hamming)
		{
			this->min_hamming_dist = std::numeric_limits<uint32_t>::max();
			for (auto n_flips = 1; n_flips <= (int)this->max_flips; n_flips++)
				this->_decode_siho_cw_recursive_hard(this->hard_Y_N.data(), V_N, 0, n_flips);
		}
		else
		{
			this->min_euclidean_dist = std::numeric_limits<float>::max();
			for (auto n_flips = 1; n_flips <= (int)this->max_flips; n_flips++)
				this->_decode_siho_cw_recursive(Y_N, V_N, 0, n_flips);
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_chase_naive<B_8,Q_8>;
template class aff3ct::module::Decoder_chase_naive<B_16,Q_16>;
template class aff3ct::module::Decoder_chase_naive<B_32,Q_32>;
template class aff3ct::module::Decoder_chase_naive<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_chase_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation