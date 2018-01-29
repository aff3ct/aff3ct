#include <chrono>
#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Perf/hard_decision.h"

#include "Decoder_chase_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_chase_std<B,R>
::Decoder_chase_std(const int K, const int N, Encoder<B> &encoder, const uint32_t max_flips, const bool hamming, 
                    const int n_frames)
: Decoder                 (K, N,                              n_frames, 1),
  Decoder_chase_naive<B,R>(K, N, encoder, max_flips, hamming, n_frames   ),
  flip_pos(max_flips)
{
	const std::string name = "Decoder_chase_std";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_chase_std<B,R>
::~Decoder_chase_std()
{
}

template <typename B, typename R>
void Decoder_chase_std<B,R>
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
void Decoder_chase_std<B,R>
::_decode_siho_cw_recursive(const R *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips)
{
	if (n_flips == max_flips)
	{
		std::copy(this->hard_Y_N.begin(), this->hard_Y_N.end(), this->X_N.begin());

		// flip the bits
		for (uint32_t f = 0; f < max_flips; f++)
			this->X_N[this->flip_pos[f]] = !this->X_N[this->flip_pos[f]];

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
void Decoder_chase_std<B,R>
::_decode_hiho_cw_recursive(const B *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips)
{
	// TODO: /!\ there is a bug in this code !!!
	if (n_flips == max_flips)
	{
		std::copy(Y_N, Y_N + this->K, this->X_N.begin());

		// flip the bits
		for (uint32_t f = 0; f < max_flips; f++)
			this->X_N[this->flip_pos[f]] = !this->X_N[this->flip_pos[f]];

		if (this->encoder.is_codeword(this->X_N.data()))
		{
			// compute the Euclidean distance between the input LLR and the current codeword
			auto cur_hamming_dist = this->compute_hamming_dist(this->X_N.data(), Y_N);

			// update the best codeword
			if (cur_hamming_dist < this->min_hamming_dist)
			{
				this->min_hamming_dist = cur_hamming_dist;
				std::copy(this->X_N.begin(), this->X_N.begin() + this->N, V_N);
			}
		}
	}
	else
	{
		for (auto flip_pos = n_flips; flip_pos < (uint32_t)this->N; flip_pos++)
		{
			this->flip_pos[n_flips] = flip_pos;
			this->_decode_hiho_cw_recursive(Y_N, V_N, n_flips +1, max_flips);
		}
	}
}

template <typename B, typename R>
void Decoder_chase_std<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	tools::hard_decide(Y_N, this->hard_Y_N.data(), this->N);

	if (this->hamming)
	{
		// TODO: /!\ there is a bug in this code !!! (_decode_hiho_cw)
		this->_decode_hiho_cw(this->hard_Y_N.data(), V_N, frame_id);
	}
	else
	{
		this->min_euclidean_dist = std::numeric_limits<float>::max();

		if (this->encoder.is_codeword(this->hard_Y_N.data()))
		{
			// compute the Euclidean distance between the input LLR and the current codeword
			auto cur_euclidean_dist = this->compute_euclidean_dist(this->hard_Y_N.data(), Y_N);

			// update the best codeword
			if (cur_euclidean_dist < this->min_euclidean_dist)
			{
				this->min_euclidean_dist = cur_euclidean_dist;
				std::copy(this->hard_Y_N.begin(), this->hard_Y_N.begin() + this->N, V_N);
			}
		}

		for (auto n_flips = 1; n_flips <= (int)this->max_flips; n_flips++)
			this->_decode_siho_cw_recursive(Y_N, V_N, 0, n_flips);
	}
}

template <typename B, typename R>
void Decoder_chase_std<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	// TODO: /!\ there is a bug in this code !!!
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
void Decoder_chase_std<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	// TODO: /!\ there is a bug in this code !!!
	this->min_hamming_dist = std::numeric_limits<uint32_t>::max();

	if (this->encoder.is_codeword(Y_N))
	{
		std::copy(Y_N, Y_N + this->N, V_N);
	}
	else
	{
		for (auto n_flips = 1; n_flips <= (int)this->max_flips; n_flips++)
			this->_decode_hiho_cw_recursive(Y_N, V_N, 0, n_flips);
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_chase_std<B_8,Q_8>;
template class aff3ct::module::Decoder_chase_std<B_16,Q_16>;
template class aff3ct::module::Decoder_chase_std<B_32,Q_32>;
template class aff3ct::module::Decoder_chase_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_chase_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation