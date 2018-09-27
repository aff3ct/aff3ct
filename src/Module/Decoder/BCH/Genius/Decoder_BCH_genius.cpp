#include <chrono>
#include <sstream>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Perf/distance/hamming_distance.h"
#include "Tools/Exception/exception.hpp"

#include "Decoder_BCH_genius.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH_genius<B,R>
::Decoder_BCH_genius(const int K, const int N, const int t, Encoder<B> &encoder, const int n_frames)
: Decoder         (K, N, n_frames, 1),
  Decoder_BCH<B,R>(K, N, t, n_frames),
  encoder(encoder), YH_N(N)
{
	const std::string name = "Decoder_BCH_genius";
	this->set_name(name);

	if (!encoder.is_memorizing())
	{
		std::stringstream message;
		message << "The given 'encoder' has to be memorizing its generated code words.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
void Decoder_BCH_genius<B,R>
::_decode(B *Y_N, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_BCH_genius<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	auto& X_N = encoder.get_X_N(frame_id);

	int n_error = (int)tools::hamming_distance(X_N.data(), Y_N, this->N);

	if (n_error <= this->t) // then copy X_N from the encoder that is Y_N corrected
		std::copy(X_N.data() + this->N - this->K, X_N.data() + this->N, V_K);
	else // then copy Y_N uncorrected
		std::copy(Y_N + this->N - this->K, Y_N + this->N, V_K);

	this->last_is_codeword[frame_id] = n_error <= this->t;
}

template <typename B, typename R>
void Decoder_BCH_genius<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	auto& X_N = encoder.get_X_N(frame_id);

	int n_error = (int)tools::hamming_distance(X_N.data(), Y_N, this->N);

	if (n_error <= this->t) // then copy X_N from the encoder that is Y_N corrected
		std::copy(X_N.data(), X_N.data() + this->N, V_N);
	else // then copy Y_N uncorrected
		std::copy(Y_N, Y_N + this->N, V_N);

	this->last_is_codeword[frame_id] = n_error <= this->t;
}

template <typename B, typename R>
void Decoder_BCH_genius<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	tools::hard_decide(Y_N, this->YH_N.data(), this->N);

	this->_decode_hiho(this->YH_N.data(), V_K, frame_id);
}

template <typename B, typename R>
void Decoder_BCH_genius<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	tools::hard_decide(Y_N, this->YH_N.data(), this->N);

	this->_decode_hiho_cw(this->YH_N.data(), V_N, frame_id);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_BCH_genius<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH_genius<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH_genius<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH_genius<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH_genius<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
