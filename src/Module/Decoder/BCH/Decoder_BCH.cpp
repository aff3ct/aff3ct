#include <chrono>
#include <sstream>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH<B, R>
::Decoder_BCH(const int K, const int N, const int t, const int n_frames)
: Decoder               (K, N, n_frames, 1),
  Decoder_SIHO_HIHO<B,R>(K, N, n_frames, 1),
  t(t), N_p2_1(tools::next_power_of_2(N) -1), last_is_codeword(n_frames)
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
bool Decoder_BCH<B, R>
::get_last_is_codeword(const int frame_id) const
{
	return last_is_codeword[frame_id < 0 ? 0 : frame_id];
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_BCH<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
