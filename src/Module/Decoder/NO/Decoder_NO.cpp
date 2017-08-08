#include <chrono>

#include "Tools/Perf/hard_decision.h"

#include "Decoder_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_NO<B,R>
::Decoder_NO(const int K, const int n_frames, const std::string name)
: Decoder_SISO_SIHO<B,R>(K, K, n_frames, 1, name)
{
}

template <typename B, typename R>
Decoder_NO<B,R>
::~Decoder_NO()
{
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_decode_siso(const R *sys, const R *par, R *ext, const int frame_id)
{
	std::copy(sys, sys + this->K, ext);
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_decode_siso(const R *Y_K1, R *Y_K2, const int frame_id)
{
	std::copy(Y_K1, Y_K1 + this->K, Y_K2);
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_decode_siho(const R *Y_K, B *V_K, const int frame_id)
{
	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(Y_K, V_K, this->K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_store_total += d_store;
}

template <typename B, typename R>
void Decoder_NO<B,R>
::_decode_siho_coded(const R *Y_K, B *V_K, const int frame_id)
{
	this->_decode_siho(Y_K, V_K, frame_id);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_NO<B_8,Q_8>;
template class aff3ct::module::Decoder_NO<B_16,Q_16>;
template class aff3ct::module::Decoder_NO<B_32,Q_32>;
template class aff3ct::module::Decoder_NO<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
