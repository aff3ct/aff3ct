#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Perf/distance/distance.h"
#include "Tools/Perf/distance/Bitwise_diff.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Bit_packer.hpp"
#include "Module/Decoder/RS/Genius/Decoder_RS_genius.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_RS_genius<B,R>
::Decoder_RS_genius(const int K, const int N, const tools::RS_polynomial_generator &GF, Encoder<B> &encoder,
                    const int n_frames)
: Decoder        (K * GF.get_m(), N * GF.get_m(), n_frames, 1),
  Decoder_RS<B,R>(K, N, GF, n_frames                         ),
  encoder        (encoder                                    ),
  X_Ns           (this->N_rs                                 )
{
	const std::string name = "Decoder_RS_genius";
	this->set_name(name);

	if (!encoder.is_memorizing())
	{
		std::stringstream message;
		message << "The given 'encoder' has to be memorizing its generated code words.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
void Decoder_RS_genius<B,R>
::_decode(S *Y_N, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_RS_genius<B,R>
::hamming_distance()
{
	int n_error = 0;
	for (auto x = this->X_Ns.begin(), y = this->YH_N.begin(); x != this->X_Ns.end(); x++, y++)
		if (*x != *y)
			n_error ++;

	return n_error;
}

template <typename B, typename R>
void Decoder_RS_genius<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const int frame_id)
{
	auto& X_N = encoder.get_X_N(frame_id);

	tools::Bit_packer::pack(X_N.data(), this->X_Ns.data(), this->N, 1, false, this->m);
	tools::Bit_packer::pack(Y_N,        this->YH_N.data(), this->N, 1, false, this->m);
	// auto n_error = this->hamming_distance();

	auto n_error = (int) tools::distance<B,tools::Bitwise_diff<B,true>>(this->X_Ns.data(), this->YH_N.data(), this->N_rs);

	// auto n_error = (int)tools::hamming_distance(X_N.data(), Y_N, this->N);

	if (n_error <= this->t) // then copy X_N from the encoder that is Y_N corrected
		std::copy(X_N.data() + this->N - this->K, X_N.data() + this->N, V_K);
	else // then copy Y_N uncorrected
		std::copy(Y_N + this->N - this->K, Y_N + this->N, V_K);

	this->last_is_codeword = n_error <= this->t;
}

template <typename B, typename R>
void Decoder_RS_genius<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id)
{
	auto& X_N = encoder.get_X_N(frame_id);

	tools::Bit_packer::pack(X_N.data(), this->X_Ns.data(), this->N, 1, false, this->m);
	tools::Bit_packer::pack(Y_N,        this->YH_N.data(), this->N, 1, false, this->m);
	auto n_error = hamming_distance();

	// int n_error = (int)tools::hamming_distance(X_N.data(), Y_N, this->N);

	if (n_error <= this->t) // then copy X_N from the encoder that is Y_N corrected
		std::copy(X_N.data(), X_N.data() + this->N, V_N);
	else // then copy Y_N uncorrected
		std::copy(Y_N, Y_N + this->N, V_N);

	this->last_is_codeword = n_error <= this->t;
}

template <typename B, typename R>
void Decoder_RS_genius<B,R>
::_decode_siho(const R *Y_N, B *V_K, const int frame_id)
{
	tools::hard_decide(Y_N, this->YH_Nb.data(), this->N);

	this->_decode_hiho(this->YH_Nb.data(), V_K, frame_id);
}

template <typename B, typename R>
void Decoder_RS_genius<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const int frame_id)
{
	tools::hard_decide(Y_N, this->YH_Nb.data(), this->N);

	this->_decode_hiho_cw(this->YH_Nb.data(), V_N, frame_id);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_RS_genius<B_8,Q_8>;
template class aff3ct::module::Decoder_RS_genius<B_16,Q_16>;
template class aff3ct::module::Decoder_RS_genius<B_32,Q_32>;
template class aff3ct::module::Decoder_RS_genius<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_RS_genius<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
