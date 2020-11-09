#include <string>

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_repetition_std<B,R>
::Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding)
: Decoder_repetition<B,R>(K,N,buffered_encoding)
{
	const std::string name = "Decoder_repetition_std";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_repetition_std<B,R>* Decoder_repetition_std<B,R>
::clone() const
{
	auto m = new Decoder_repetition_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
int Decoder_repetition_std<B,R>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	for (auto i = 0; i < this->K; i++)
	{
		ext[i] = sys[i];
		for (auto j = 0; j < this->rep_count; j++)
			// ext[i] += (par[j*this->K +i] > 0) ? 1 : -1; // hard decision
			ext[i] += par[j*this->K +i]; // soft decision
	}

	return 0;
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
