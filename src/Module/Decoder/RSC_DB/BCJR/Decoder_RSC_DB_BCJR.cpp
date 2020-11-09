#include <cmath>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_RSC_DB_BCJR<B,R>
::Decoder_RSC_DB_BCJR(const int K,
                      const std::vector<std::vector<int>> &trellis,
                      const bool buffered_encoding)
: Decoder_SISO<B,R>(K, 2 * K                                ),
  n_states         ((int)trellis[0].size()/4                ),
  n_ff             ((int)std::log2(n_states)                ),
  buffered_encoding(buffered_encoding                       ),
  trellis          (trellis                                 ),
  sys              (2*K                                     ),
  par              (  K                                     ),
  ext              (2*K                                     ),
  s                (  K                                     ),
  alpha_mp         (n_states                                ),
  beta_mp          (n_states                                ),
  alpha            (K/2 + 1, std::vector<R>(n_states    , 0)),
  beta             (K/2 + 1, std::vector<R>(n_states    , 0)),
  gamma            (K/2    , std::vector<R>(n_states * 4, 0))
{
	const std::string name = "Decoder_RSC_DB_BCJR";
	this->set_name(name);

	if (!tools::is_power_of_2(n_states))
	{
		std::stringstream message;
		message << "'n_states' has to be a power of 2 ('n_states' = " << n_states << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_RSC_DB_BCJR<B,R>* Decoder_RSC_DB_BCJR<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_RSC_DB_BCJR<B,R>
::_load(const R *Y_N)
{
	notify_new_frame();

	if (buffered_encoding)
	{
		for (auto i = 0; i < this->K / 2; i++)
		{
			R a = tools::div2(Y_N[2*i  ]);
			R b = tools::div2(Y_N[2*i+1]);
			sys[4*i + 0] =  a + b;
			sys[4*i + 1] =  a - b;
			sys[4*i + 2] = -a + b;
			sys[4*i + 3] = -a - b;
		}
		for (auto i = 0; i < this->K; i++)
			par[i] = tools::div2(Y_N[this->K + i]);
	}
	else
	{
		for (auto i = 0; i < this->K / 2; i++)
		{
			R a = tools::div2(Y_N[4*i  ]);
			R b = tools::div2(Y_N[4*i+1]);
			sys[4*i + 0] =  a + b;
			sys[4*i + 1] =  a - b;
			sys[4*i + 2] = -a + b;
			sys[4*i + 3] = -a - b;

			par[2*i  ] = tools::div2(Y_N[4*i + 2]);
			par[2*i+1] = tools::div2(Y_N[4*i + 3]);
		}
	}
}


template <typename B, typename R>
int Decoder_RSC_DB_BCJR<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	auto status = this->_decode_siso_alt(sys.data(), par.data(), ext.data(), frame_id);
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto i = 0; i < this->K; i+=2)
	{
		s[i  ] = (  std::max(ext[2*i+2] + sys[2*i+2], ext[2*i+3] + sys[2*i+3])
		          - std::max(ext[2*i+0] + sys[2*i+0], ext[2*i+1] + sys[2*i+1])  ) > 0;
		s[i+1] = (  std::max(ext[2*i+1] + sys[2*i+1], ext[2*i+3] + sys[2*i+3])
		          - std::max(ext[2*i+0] + sys[2*i+0], ext[2*i+2] + sys[2*i+2])  ) > 0;
	}
	_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return status;
}

template <typename B, typename R>
void Decoder_RSC_DB_BCJR<B,R>
::_store(B *V_K) const
{
	std::copy(s.begin(), s.begin() + this->K, V_K);
}


template <typename B, typename R>
int Decoder_RSC_DB_BCJR<B,R>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	__init_alpha_beta();
	__fwd_recursion(sys, par);
	__bwd_recursion(sys, par, ext);
	__save_alpha_beta();

	return 0;
}

template <typename B, typename R>
void Decoder_RSC_DB_BCJR<B,R>
::__init_alpha_beta()
{
	for (auto s = 0; s<n_states; s++)
	{
		alpha[           0][s] = alpha_mp[s];
		beta[beta.size()-1][s] =  beta_mp[s];
	}
}

template <typename B, typename R>
void Decoder_RSC_DB_BCJR<B,R>
::__save_alpha_beta()
{
	for (auto s = 0; s < n_states; s++)
	{
		alpha_mp[s] = alpha[alpha.size()-1][s];
		 beta_mp[s] = beta[0][s];
	}
}

template <typename B, typename R>
void Decoder_RSC_DB_BCJR<B,R>
::notify_new_frame()
{
	for (auto s = 0; s < n_states; s++)
	{
		alpha_mp[s] = (R)0;
		beta_mp [s] = (R)0;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_RSC_DB_BCJR<B_8,Q_8>;
template class aff3ct::module::Decoder_RSC_DB_BCJR<B_16,Q_16>;
template class aff3ct::module::Decoder_RSC_DB_BCJR<B_32,Q_32>;
template class aff3ct::module::Decoder_RSC_DB_BCJR<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_RSC_DB_BCJR<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
