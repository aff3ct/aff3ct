#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH<B, R>
::Decoder_BCH(const int K, const int N, const int t)
: Decoder_SIHO<B,R>(K, N),
  t(t), N_p2_1(tools::next_power_of_2(N) -1), last_is_codeword(this->n_frames)
{
	const std::string name = "Decoder_BCH";
	this->set_name(name);

	if (K <= 3)
	{
		std::stringstream message;
		message << "'K' has to be greater than 3 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_BCH<B,R>* Decoder_BCH<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
bool Decoder_BCH<B, R>
::get_last_is_codeword(const int frame_id) const
{
	return last_is_codeword[frame_id < 0 ? 0 : frame_id];
}

template <typename B, typename R>
void Decoder_BCH<B, R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SIHO<B,R>::set_n_frames(n_frames);

		const auto old_last_is_codeword_size = this->last_is_codeword.size();
		const auto new_last_is_codeword_size = (old_last_is_codeword_size / old_n_frames) * n_frames;
		this->last_is_codeword.resize(new_last_is_codeword_size);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_BCH<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
