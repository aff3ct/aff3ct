#include "Encoder_coset.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_coset<B>
::Encoder_coset(const int K, const int N, const int seed, const int n_frames)
: Encoder<B>(K, N, n_frames), rd_engine(seed + 1024), uniform_dist(0, 1)
{
	const std::string name = "Encoder_coset";
	this->set_name(name);
}

template <typename B>
void Encoder_coset<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	std::copy(U_K, U_K + this->K, X_N);

	for (auto i = this->K; i < this->N; i++)
		X_N[i] = (B)this->uniform_dist(this->rd_engine);
}

template <typename B>
const std::vector<uint32_t>& Encoder_coset<B>
::get_info_bits_pos() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
bool Encoder_coset<B>
::is_sys() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_coset<B_8>;
template class aff3ct::module::Encoder_coset<B_16>;
template class aff3ct::module::Encoder_coset<B_32>;
template class aff3ct::module::Encoder_coset<B_64>;
#else
template class aff3ct::module::Encoder_coset<B>;
#endif
// ==================================================================================== explicit template instantiation
