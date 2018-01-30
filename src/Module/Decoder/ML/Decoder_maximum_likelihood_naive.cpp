#include <chrono>
#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Perf/hard_decision.h"

#include "Decoder_maximum_likelihood_naive.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_maximum_likelihood_naive<B,R>
::Decoder_maximum_likelihood_naive(const int K, const int N, Encoder<B> &encoder, const bool hamming, const int n_frames)
: Decoder                        (K, N,          n_frames, 1),
  Decoder_maximum_likelihood<B,R>(K, N, encoder, n_frames   ),
  hamming(hamming),
  x_max(0),
  min_euclidean_dist(std::numeric_limits<float>::max()),
  min_hamming_dist(std::numeric_limits<uint32_t>::max())
{
	const std::string name = "Decoder_maximum_likelihood_naive";
	this->set_name(name);

	if (N > 64)
	{
		std::stringstream message;
		message << "'N' has to be smaller or equal to 64 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// determine the maximum sequence 'x' of codeword bits
	if (N == 64)
		this->x_max = std::numeric_limits<uint64_t>::max();
	else
		this->x_max = ((uint64_t)1 << (uint64_t)(N)) -1;
}

template <typename B, typename R>
Decoder_maximum_likelihood_naive<B,R>
::~Decoder_maximum_likelihood_naive()
{
}

template <typename B, typename R>
void Decoder_maximum_likelihood_naive<B,R>
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
void Decoder_maximum_likelihood_naive<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	// compute Hamming distance instead of Euclidean distance
	if (hamming)
	{
		tools::hard_decide(Y_N, this->hard_Y_N.data(), this->N);
		this->_decode_hiho_cw(this->hard_Y_N.data(), V_N, frame_id);
	}
	else
	{
		this->min_euclidean_dist = std::numeric_limits<float>::max();

		// for all the possible sequences of information bits
		for (uint64_t x = 0; x <= this->x_max; x++)
		{
			// unpack x and check if the sequence of bits is a valid codeword
			std::fill(this->X_N.begin(), this->X_N.end(), (B)0);
			auto data = (uint64_t*)this->X_N.data();
			data[0] = x;
			tools::Bit_packer<B>::unpack(this->X_N.data(), this->N);
			if (this->encoder.is_codeword(this->X_N.data()))
			{
				// compute the Euclidean distance between the input LLR and the current codeword
				auto cur_euclidean_dist = this->compute_euclidean_dist(this->X_N.data(), Y_N);

				// update the best codeword
				if (cur_euclidean_dist < this->min_euclidean_dist)
				{
					this->min_euclidean_dist = cur_euclidean_dist;
					std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);
				}
			}

			// trick to avoid infinite loop when x_max is the max 64-bit unsigned integer
			if (this->x_max == std::numeric_limits<uint64_t>::max())
				break;
		}
	}
}

template <typename B, typename R>
void Decoder_maximum_likelihood_naive<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	if (!this->encoder.is_sys())
	{
		std::stringstream message;
		message << "'encoder.is_sys()' has to be true.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->_decode_hiho_cw(Y_N, this->best_X_N.data(), frame_id);
	
	const auto &info_bits_pos = this->encoder.get_info_bits_pos();
	for (auto k = 0; k < this->K; k++)
		V_K[k] = this->best_X_N[info_bits_pos[k]];
}

template <typename B, typename R>
void Decoder_maximum_likelihood_naive<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	this->min_hamming_dist = std::numeric_limits<uint32_t>::max();

	// for all the possible sequences of information bits
	for (uint64_t x = 0; x <= this->x_max; x++)
	{
		// unpack x and check if the sequence of bits is a valid codeword
		std::fill(this->X_N.begin(), this->X_N.end(), (B)0);
		auto data = (uint64_t*)this->X_N.data();
		data[0] = x;
		tools::Bit_packer<B>::unpack(this->X_N.data(), this->N);
		if (this->encoder.is_codeword(this->X_N.data()))
		{
			// compute the Hamming distance between the input bits and the current codeword
			auto cur_hamming_dist = this->compute_hamming_dist(this->X_N.data(), Y_N);

			// update the best codeword
			if (cur_hamming_dist < this->min_hamming_dist)
			{
				this->min_hamming_dist = cur_hamming_dist;
				std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);
			}
		}

		// trick to avoid infinite loop when x_max is the max 64-bit unsigned integer
		if (x_max == std::numeric_limits<uint64_t>::max())
			break;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_maximum_likelihood_naive<B_8,Q_8>;
template class aff3ct::module::Decoder_maximum_likelihood_naive<B_16,Q_16>;
template class aff3ct::module::Decoder_maximum_likelihood_naive<B_32,Q_32>;
template class aff3ct::module::Decoder_maximum_likelihood_naive<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_maximum_likelihood_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation