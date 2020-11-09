#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_repetition<B,R>
::Decoder_repetition(const int& K, const int& N, const bool buffered_encoding)
: Decoder_SISO<B,R>(K, N),
  rep_count((N/K) -1), buffered_encoding(buffered_encoding), sys(K), par(K * rep_count), ext(K)
{
	const std::string name = "Decoder_repetition";
	this->set_name(name);

	if (N % K)
	{
		std::stringstream message;
		message << "'K' has to be a multiple of 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_repetition<B,R>* Decoder_repetition<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_repetition<B,R>
::_load(const R *Y_N)
{
	if (!buffered_encoding)
	{
		for (auto i = 0; i < this->K; i++)
		{
			const auto off1 = i * (rep_count +1);
			const auto off2 = off1 +1;

			sys[i] = Y_N[off1];
			for (auto j = 0; j < rep_count; j++)
				par[j*this->K +i] = Y_N[off2 +j];
		}
	}
	else
	{
		std::copy(Y_N, Y_N + this->K, sys.begin());
		for (auto i = 0; i < rep_count; i++)
			std::copy(Y_N + (i +1) * this->K, Y_N + (i +2) * this->K, par.begin() + (i +0) * this->K);
	}
}

template <typename B, typename R>
int Decoder_repetition<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode_siso_alt(sys.data(), par.data(), ext.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	tools::hard_decide(ext.data(), V_K, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return status;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_repetition<B_8,Q_8>;
template class aff3ct::module::Decoder_repetition<B_16,Q_16>;
template class aff3ct::module::Decoder_repetition<B_32,Q_32>;
template class aff3ct::module::Decoder_repetition<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
