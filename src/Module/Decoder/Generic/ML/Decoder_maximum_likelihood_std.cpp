#include <chrono>
#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Perf/common/hard_decide.h"

#include "Decoder_maximum_likelihood_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_maximum_likelihood_std<B,R>
::Decoder_maximum_likelihood_std(const int K, const int N, Encoder<B> &encoder, const bool hamming, const int n_frames)
: Decoder                        (K, N,          n_frames, 1),
  Decoder_maximum_likelihood<B,R>(K, N, encoder, n_frames   ),
  hamming(hamming),
  u_max(0),
  min_euclidean_dist(std::numeric_limits<float>::max()),
  min_hamming_dist(std::numeric_limits<uint32_t>::max())
{
	const std::string name = "Decoder_maximum_likelihood_std";
	this->set_name(name);

	if (K > 64)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 64 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// determine the maximum sequence 'u' of information bits
	if (K == 64)
		this->u_max = std::numeric_limits<uint64_t>::max();
	else
		this->u_max = ((uint64_t)1 << (uint64_t)(K)) -1;
}

template <typename B, typename R>
void Decoder_maximum_likelihood_std<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	this->_decode_siho_cw(Y_N, this->best_X_N.data(), frame_id);
	std::copy(this->best_U_K.begin(), this->best_U_K.end(), V_K);
}

template <typename B, typename R>
void Decoder_maximum_likelihood_std<B,R>
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
		for (uint64_t u = 0; u <= this->u_max; u++)
		{
			// convert the information bits to the codeword
			std::fill(this->U_K.begin(), this->U_K.end(), (B)0);
			auto data = (uint64_t*)this->U_K.data();
			data[0] = u;
			tools::Bit_packer::unpack(this->U_K.data(), this->K);
			this->encoder.encode(this->U_K.data(), this->X_N.data(), 0);

			// compute the Euclidean distance between the input LLR and the current codeword
			auto cur_euclidean_dist = this->compute_euclidean_dist(this->X_N.data(), Y_N);

			// update the best codeword
			if (cur_euclidean_dist < this->min_euclidean_dist)
			{
				this->min_euclidean_dist = cur_euclidean_dist;
				std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);
				std::copy(this->U_K.begin(), this->U_K.begin() + this->K, this->best_U_K.begin());
			}

			// trick to avoid infinite loop when u_max is the max 64-bit unsigned integer
			if (this->u_max == std::numeric_limits<uint64_t>::max())
				break;
		}
	}
}

template <typename B, typename R>
void Decoder_maximum_likelihood_std<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	this->_decode_hiho_cw(Y_N, this->best_X_N.data(), frame_id);
	std::copy(this->best_U_K.begin(), this->best_U_K.end(), V_K);
}

template <typename B, typename R>
void Decoder_maximum_likelihood_std<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	this->min_hamming_dist = std::numeric_limits<uint32_t>::max();

	// for all the possible sequences of information bits
	for (uint64_t u = 0; u <= this->u_max; u++)
	{
		// convert the information bits to the codeword
		std::fill(this->U_K.begin(), this->U_K.end(), (B)0);
		auto data = (uint64_t*)this->U_K.data();
		data[0] = u;
		tools::Bit_packer::unpack(this->U_K.data(), this->K);
		this->encoder.encode(this->U_K.data(), this->X_N.data(), 0);

		// compute the Hamming distance between the input bits and the current codeword
		auto cur_hamming_dist = this->compute_hamming_dist(this->X_N.data(), Y_N);

		// update the best codeword
		if (cur_hamming_dist < this->min_hamming_dist)
		{
			this->min_hamming_dist = cur_hamming_dist;
			std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);
			std::copy(this->U_K.begin(), this->U_K.begin() + this->K, this->best_U_K.begin());
		}

		// trick to avoid infinite loop when u_max is the max 64-bit unsigned integer
		if (u_max == std::numeric_limits<uint64_t>::max())
			break;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_maximum_likelihood_std<B_8,Q_8>;
template class aff3ct::module::Decoder_maximum_likelihood_std<B_16,Q_16>;
template class aff3ct::module::Decoder_maximum_likelihood_std<B_32,Q_32>;
template class aff3ct::module::Decoder_maximum_likelihood_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_maximum_likelihood_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation