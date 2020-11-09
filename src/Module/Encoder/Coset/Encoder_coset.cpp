#include <string>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_coset<B>
::Encoder_coset(const int K, const int N, const int seed)
: Encoder<B>(K, N), rd_engine(seed), uniform_dist(0, 1)
{
	const std::string name = "Encoder_coset";
	this->set_name(name);
}

template <typename B>
Encoder_coset<B>* Encoder_coset<B>
::clone() const
{
	auto m = new Encoder_coset(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_coset<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
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

template <typename B>
void Encoder_coset<B>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_coset<B_8>;
template class aff3ct::module::Encoder_coset<B_16>;
template class aff3ct::module::Encoder_coset<B_32>;
template class aff3ct::module::Encoder_coset<B_64>;
#else
template class aff3ct::module::Encoder_coset<B>;
#endif
// ==================================================================================== explicit template instantiation
