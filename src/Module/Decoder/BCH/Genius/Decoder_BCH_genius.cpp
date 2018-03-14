#include <chrono>
#include <sstream>

#include "Tools/Perf/hard_decision.h"
#include "Tools/Exception/exception.hpp"

#include "Decoder_BCH_genius.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH_genius<B, R>
::Decoder_BCH_genius(const int& K, const int& N, const tools::BCH_polynomial_generator &GF_poly, Encoder<B> &encoder, const int n_frames)
: Decoder         (K, N, n_frames, 1),
  Decoder_BCH<B,R>(K, N, GF_poly, n_frames),
  encoder(encoder),
  error_pos(this->t)
{
	const std::string name = "Decoder_BCH_genius";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_BCH_genius<B, R>
::~Decoder_BCH_genius()
{
}

template <typename B, typename R>
void Decoder_BCH_genius<B, R>
::_decode(B *Y_N, const int frame_id)
{
	auto& X_N = encoder.get_X_N(frame_id);

	int n_error = 0;
	for(int i = 0; i < this->N; i++)
	{
		if (X_N[i] != Y_N[i])
		{
			error_pos[n_error] = i;
			n_error ++;
		}

		if (n_error > this->t)
			break;
	}

	if (n_error <= this->t)
	{
		for(int i = 0; i < n_error; i++)
			Y_N[error_pos[i]] = !Y_N[error_pos[i]];
	}
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
