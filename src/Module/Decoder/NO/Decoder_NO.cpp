#include <algorithm>
#include <string>

#include "Tools/Perf/common/hard_decide.h"
#include "Module/Decoder/NO/Decoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_NO<B,R>
::Decoder_NO(const int K)
: Decoder_SISO<B,R>(K, K)
{
	const std::string name = "Decoder_NO";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_NO<B,R>* Decoder_NO<B,R>
::clone() const
{
	auto m = new Decoder_NO(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
int Decoder_NO<B,R>
::_decode_siso(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	std::copy(sys, sys + this->K, ext);

	return 0;
}

template <typename B, typename R>
int Decoder_NO<B,R>
::_decode_siso(const R *Y_K1, R *Y_K2, const size_t frame_id)
{
	std::copy(Y_K1, Y_K1 + this->K, Y_K2);

	return 0;
}

template <typename B, typename R>
int Decoder_NO<B,R>
::_decode_siho(const R *Y_K, B *V_K, const size_t frame_id)
{
//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide_unk(Y_K, V_K, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store, d_store);

	return 0;
}

template <typename B, typename R>
int Decoder_NO<B,R>
::_decode_siho_cw(const R *Y_K, B *V_K, const size_t frame_id)
{
//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide_unk(Y_K, V_K, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store, d_store);

	return 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_NO<B_8,Q_8>;
template class aff3ct::module::Decoder_NO<B_16,Q_16>;
template class aff3ct::module::Decoder_NO<B_32,Q_32>;
template class aff3ct::module::Decoder_NO<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
