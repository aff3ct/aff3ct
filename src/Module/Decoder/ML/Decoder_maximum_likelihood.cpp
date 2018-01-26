#include <chrono>
#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"

#include "Decoder_maximum_likelihood.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_maximum_likelihood<B,R>
::Decoder_maximum_likelihood(const int K, const int N, Encoder<B> &encoder, const int n_frames)
: Decoder          (K, N, n_frames, 1),
  Decoder_SIHO<B,R>(K, N, n_frames, 1),
  encoder(encoder),
  U_K(K+7), // +7 to avoid segmentation fault when casting B to uint64_t
  X_N(N),
  best_U_K(K),
  best_X_N(N),
  min_euclidean_dist(std::numeric_limits<float>::max())
{
	const std::string name = "Decoder_maximum_likelihood";
	this->set_name(name);

	if (K > 64)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 64 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (encoder.get_K() != K)
	{
		std::stringstream message;
		message << "'encoder.get_K()' has to be equal to 'K' ('encoder.get_K()' = " << encoder.get_K() 
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (encoder.get_N() != N)
	{
		std::stringstream message;
		message << "'encoder.get_N()' has to be equal to 'N' ('encoder.get_N()' = " << encoder.get_N() 
		        << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (encoder.get_n_frames() != 1)
	{
		std::stringstream message;
		message << "'encoder.get_n_frames()' has to be equal to 1 ('encoder.get_n_frames()' = " 
		        << encoder.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_maximum_likelihood<B,R>
::~Decoder_maximum_likelihood()
{
}

template <typename B, typename R>
void Decoder_maximum_likelihood<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	this->_decode_siho_cw(Y_N, this->best_X_N.data(), frame_id);
	std::copy(this->best_U_K.begin(), this->best_U_K.end(), V_K);
}

template <typename B, typename R>
void Decoder_maximum_likelihood<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	this->min_euclidean_dist = std::numeric_limits<float>::max();

	// determine the maximum sequence 'u' of information bits
	uint64_t u_max = 0;
	if (this->K > 0)
	{
		if (this->K == 64)
			u_max = std::numeric_limits<uint64_t>::max();
		else
			u_max = ((uint64_t)1 << (uint64_t)(this->K)) -1;
	}

	// for all the possible sequences of information bits
	for (uint64_t u = 0; u <= u_max; u++)
	{
		// convert information bits to codeword
		std::fill(U_K.begin(), U_K.end(), (B)0);
		auto data = (uint64_t*)this->U_K.data();
		data[0] = u;
		tools::Bit_packer<B>::unpack(this->U_K.data(), this->K);
		this->encoder.encode(this->U_K.data(), this->X_N.data());

		// compute the Euclidean distance between the input LLR and the current codeword
		float cur_euclidean_dist = 0.f;
		for (auto n = 0; n < this->N; n++)
		{
			auto tmp = (this->X_N[n] ? -1.f : +1.f) - (float)Y_N[n];
			cur_euclidean_dist += tmp * tmp;
		}

		// we can avoid the sqrt because of the monotonicity of the sqrt function
		// cur_euclidean_dist = std::sqrt(cur_euclidean_dist);

		// update the codeword
		if (cur_euclidean_dist < this->min_euclidean_dist)
		{
			this->min_euclidean_dist = cur_euclidean_dist;
			std::copy(this->X_N.begin(), this->X_N.end(), V_N);
			std::copy(this->U_K.begin(), this->U_K.begin() + this->K, this->best_U_K.begin());
		}

		// trick to avoid infinite loop when u_max is the max 64-bit unsigned integer
		if (u_max == std::numeric_limits<uint64_t>::max())
			break;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_maximum_likelihood<B_8,Q_8>;
template class aff3ct::module::Decoder_maximum_likelihood<B_16,Q_16>;
template class aff3ct::module::Decoder_maximum_likelihood<B_32,Q_32>;
template class aff3ct::module::Decoder_maximum_likelihood<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_maximum_likelihood<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
