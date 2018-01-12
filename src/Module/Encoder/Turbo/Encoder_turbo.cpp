#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Encoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

// [sysn | tail sysn | pn | tail pn | tail sysi | pi | tail pi]

template <typename B>
Encoder_turbo<B>
::Encoder_turbo(const int& K, const int& N, const Interleaver<B> &pi,
                Encoder<B> &enco_n, Encoder<B> &enco_i, const int n_frames)
: Encoder<B>(K, N, n_frames),
  pi        (pi),
  enco_n    (enco_n),
  enco_i    (enco_i),
  U_K_i     (K * n_frames),
  X_N_tmp   ((enco_n.get_N() >= enco_i.get_N() ? enco_n.get_N() : enco_i.get_N()) * n_frames)
{
	const std::string name = "Encoder_turbo";
	this->set_name(name);
	
	if (N - (enco_n.tail_length() + enco_i.tail_length()) != 3 * K)
	{
		std::stringstream message;
		message << "'N' - ('enco_n.tail_length()' + 'enco_i.tail_length()') has to be equal to 3 * 'K' ('N' = " << N
		        << ", 'enco_n.tail_length()' = " << enco_n.tail_length()
		        << ", 'enco_i.tail_length()' = " << enco_i.tail_length()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Encoder_turbo<B>
::encode(const B *U_K, B *X_N)
{
	pi.interleave(U_K, U_K_i.data(), 0, this->n_frames);

	enco_n.encode(U_K, X_N_tmp.data());

	for (auto f = 0; f < this->n_frames; f++)
		std::copy(X_N_tmp.data() + f * enco_n.get_N(),
		          X_N_tmp.data() + f * enco_n.get_N() + enco_n.get_N(),
		          X_N            + f * this->N);

	enco_i.encode(U_K_i.data(), X_N_tmp.data());

	for (auto f = 0; f < this->n_frames; f++)
		std::copy(X_N_tmp.data() + f * enco_i.get_N() + enco_i.get_K(),
		          X_N_tmp.data() + f * enco_i.get_N() + enco_i.get_N(),
		          X_N            + f * this->N + enco_n.get_N());
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_turbo<B_8>;
template class aff3ct::module::Encoder_turbo<B_16>;
template class aff3ct::module::Encoder_turbo<B_32>;
template class aff3ct::module::Encoder_turbo<B_64>;
#else
template class aff3ct::module::Encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation
