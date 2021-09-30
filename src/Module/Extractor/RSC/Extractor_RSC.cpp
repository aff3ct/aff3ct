#include <algorithm>
#include <memory>

#include "Module/Extractor/RSC/Extractor_RSC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Extractor_RSC<B,Q>
::Extractor_RSC(const int K, const int N, const int tail_length, const bool buffered_encoding)
: Extractor<B,Q>(K, N, tail_length),
  buffered_encoding(buffered_encoding), info_bits_pos(K, 0)
{
	const std::string name = "Extractor_RSC";
	this->set_name(name);

	if (buffered_encoding)
		for (auto i = 0; i < K; i++)
			info_bits_pos[i] = i;
	else
		for (auto i = 0; i < K; i++)
			info_bits_pos[i] = 2*i;
}

template <typename B, typename Q>
Extractor_RSC<B,Q>* Extractor_RSC<B,Q>
::clone() const
{
	auto m = new Extractor_RSC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Extractor_RSC<B,Q>
::_get_sys_and_par_llr(const Q* Y_N, Q* sys, Q* par, const size_t frame_id)
{
	const auto tb_2 = this->tail_length / 2;
	const auto K    = this->K;
	const auto N    = this->N;

	if (buffered_encoding)
	{
		std::copy(Y_N,            Y_N + K + tb_2, sys);
		std::copy(Y_N + K + tb_2, Y_N + N,        par);
	}
	else
	{
		for (auto i = 0; i < K + tb_2; i++)
		{
			sys[i] = Y_N[i*2 +0];
			par[i] = Y_N[i*2 +1];
		}
	}
}

template <typename B, typename Q>
void Extractor_RSC<B,Q>
::_get_sys_llr(const Q* Y_N, Q* sys, const size_t frame_id)
{
	if (buffered_encoding)
		std::copy(Y_N, Y_N + this->K, sys);
	else
		for (auto i = 0; i < this->K; i++)
			sys[i] = Y_N[i*2];
}

template <typename B, typename Q>
void Extractor_RSC<B,Q>
::_add_sys_and_ext_llr(const Q* ext, Q* Y_N, const size_t frame_id)
{
	if (buffered_encoding)
		for (auto i = 0; i < this->K; i++)
			Y_N[i] += ext[i];
	else
		for (auto i = 0; i < this->K; i++)
			Y_N[i*2] += ext[i];
}

template <typename B, typename Q>
void Extractor_RSC<B,Q>
::_get_sys_bit(const Q *Y_N, B *V_K, const size_t frame_id)
{
	if (buffered_encoding)
		for (auto i = 0; i < this->K; i++)
			V_K[i] = Y_N[i] >= 0 ? (B)0 : (B)1;
	else
		for (auto i = 0; i < this->K; i++)
			V_K[i] = Y_N[2*i] >= 0 ? (B)0 : (B)1;
}

template <typename B, typename Q>
const std::vector<uint32_t>& Extractor_RSC<B,Q>
::get_info_bits_pos()
{
	return info_bits_pos;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Extractor_RSC<B_8,Q_8>;
template class aff3ct::module::Extractor_RSC<B_16,Q_16>;
template class aff3ct::module::Extractor_RSC<B_32,Q_32>;
template class aff3ct::module::Extractor_RSC<B_64,Q_64>;
#else
template class aff3ct::module::Extractor_RSC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
