#include <chrono>
#include <iostream>

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
: Decoder                        (K, N,          n_frames, 1),
  Decoder_maximum_likelihood<B,R>(K, N, encoder, n_frames   ),
  max_flips(max_flips),
  hamming(hamming),
  min_euclidean_dist(std::numeric_limits<float>::max()),
  min_hamming_dist(std::numeric_limits<uint32_t>::max())
{
	const std::string name = "Decoder_chase_naive";
	this->set_name(name);

	if (max_flips > (uint32_t)N)
	{
		std::stringstream message;
		message << "'max_flips' has to be smaller or equal to 'N' ('max_flips' = " << max_flips 
		        << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
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
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	tools::hard_decide(Y_N, this->hard_Y_N.data(), this->N);

	// compute Hamming distance instead of Euclidean distance
	if (hamming)
	{
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
		{
			if (n_flips == 1)
			{
				for (auto flip1_pos = 0; flip1_pos < this->N; flip1_pos++)
				{
					std::copy(this->hard_Y_N.begin(), this->hard_Y_N.end(), this->X_N.begin());
					this->X_N[flip1_pos] = !this->X_N[flip1_pos];

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
			}
			else if (n_flips == 2)
			{
				for (auto flip1_pos = 0; flip1_pos < this->N; flip1_pos++)
					for (auto flip2_pos = flip1_pos +1; flip2_pos < this->N; flip2_pos++)
					{
						std::copy(this->hard_Y_N.begin(), this->hard_Y_N.end(), this->X_N.begin());
						this->X_N[flip1_pos] = !this->X_N[flip1_pos];
						this->X_N[flip2_pos] = !this->X_N[flip2_pos];

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
			}
			else if (n_flips == 3)
			{
				for (auto flip1_pos = 0; flip1_pos < this->N; flip1_pos++)
					for (auto flip2_pos = flip1_pos +1; flip2_pos < this->N; flip2_pos++)
						for (auto flip3_pos = flip2_pos +1; flip3_pos < this->N; flip3_pos++)
						{
							std::copy(this->hard_Y_N.begin(), this->hard_Y_N.end(), this->X_N.begin());
							this->X_N[flip1_pos] = !this->X_N[flip1_pos];
							this->X_N[flip2_pos] = !this->X_N[flip2_pos];
							this->X_N[flip3_pos] = !this->X_N[flip3_pos];

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
			}
			else if (n_flips == 4)
			{
				for (auto flip1_pos = 0; flip1_pos < this->N; flip1_pos++)
					for (auto flip2_pos = flip1_pos +1; flip2_pos < this->N; flip2_pos++)
						for (auto flip3_pos = flip2_pos +1; flip3_pos < this->N; flip3_pos++)
							for (auto flip4_pos = flip3_pos +1; flip4_pos < this->N; flip4_pos++)
							{
								std::copy(this->hard_Y_N.begin(), this->hard_Y_N.end(), this->X_N.begin());
								this->X_N[flip1_pos] = !this->X_N[flip1_pos];
								this->X_N[flip2_pos] = !this->X_N[flip2_pos];
								this->X_N[flip3_pos] = !this->X_N[flip3_pos];
								this->X_N[flip4_pos] = !this->X_N[flip4_pos];
								
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
			}
			else
			{
				std::stringstream message;
				message << "'max_flips' unsupported size ('max_flips' = " << max_flips << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}
}

template <typename B, typename R>
void Decoder_chase_naive<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_chase_naive<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
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