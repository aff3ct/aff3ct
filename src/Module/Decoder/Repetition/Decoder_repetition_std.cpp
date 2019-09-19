#include <string>

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_repetition_std<B,R>
::Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding, const int n_frames)
: Decoder(K, N, n_frames, 1),
  Decoder_repetition<B,R>(K,N,buffered_encoding, n_frames)
{
	const std::string name = "Decoder_repetition_std";
	this->set_name(name);
}

template <typename B, typename R>
void Decoder_repetition_std<B,R>
::_decode_siso(const R *sys, const R *par, R *ext, const int frame_id)
{
	for (auto i = 0; i < this->K; i++)
	{
		ext[i] = sys[i];
		for (auto j = 0; j < this->rep_count; j++)
			// ext[i] += (par[j*this->K +i] > 0) ? 1 : -1; // hard decision
			ext[i] += par[j*this->K +i]; // soft decision
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_repetition_std<B_8,Q_8>;
template class aff3ct::module::Decoder_repetition_std<B_16,Q_16>;
template class aff3ct::module::Decoder_repetition_std<B_32,Q_32>;
template class aff3ct::module::Decoder_repetition_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_repetition_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
