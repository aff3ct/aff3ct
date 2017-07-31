#include <chrono>
#include <limits>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_RSC_DB_BCJR.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_max<R> MAX>
Decoder_RSC_DB_BCJR<B,R,MAX>
::Decoder_RSC_DB_BCJR(const int K,
                      const std::vector<std::vector<int>> &trellis,
                      const bool buffered_encoding,
                      const int n_frames,
                      const std::string name)
: Decoder_SISO<B,R>(K, 2 * K, n_frames, 1, name),
  n_states         ((int)trellis[0].size()/4                 ),
  n_ff             ((int)std::log2(n_states)                 ),
  buffered_encoding(buffered_encoding                        ),
  trellis          (trellis                                  ),
  sys              (2*K                                      ),
  par              (  K                                      ),
  ext              (2*K                                      ),
  s                (  K                                      ),
  alpha_mp         (n_states                                 ),
  beta_mp          (n_states                                 ),
  alpha            (K/2 + 1, mipp::vector<R>(n_states    , 0)),
  beta             (K/2 + 1, mipp::vector<R>(n_states    , 0)),
  gamma            (K/2    , mipp::vector<R>(n_states * 4, 0))
{
	if (!tools::is_power_of_2(n_states))
	{
		std::stringstream message;
		message << "'n_states' has to be a power of 2 ('n_states' = " << n_states << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
Decoder_RSC_DB_BCJR<B,R,MAX>
::~Decoder_RSC_DB_BCJR()
{
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
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
		for(auto i=0; i<this->K; i++)
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

			par[2*i  ] = Y_N[4*i + 2];
			par[2*i+2] = Y_N[4*i + 3];
		}
	}
}


template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::_hard_decode(const R *Y_N, B *V_K, const int frame_id)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	_load(Y_N);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_soft_decode(sys.data(), par.data(), ext.data(), frame_id);
	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	for (auto i = 0; i < this->K; i+=2)
	{
		s[i  ] = (  std::max(ext[2*i+2] + sys[2*i+2], ext[2*i+3] + sys[2*i+3])
		          - std::max(ext[2*i+0] + sys[2*i+0], ext[2*i+1] + sys[2*i+1])  ) > 0;
		s[i+1] = (  std::max(ext[2*i+1] + sys[2*i+1], ext[2*i+3] + sys[2*i+3])
		          - std::max(ext[2*i+0] + sys[2*i+0], ext[2*i+2] + sys[2*i+2])  ) > 0;
	}
	_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::_store(B *V_K) const
{
	std::copy(s.begin(), s.begin() + this->K, V_K);
}


template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::_soft_decode(const R *sys, const R *par, R *ext, const int frame_id)
{
	__init_alpha_beta();
	__fwd_recursion(sys, par);
	__bwd_recursion(sys, par, ext);
	__save_alpha_beta();
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::__init_alpha_beta()
{
	for (auto s = 0; s<n_states; s++)
	{
		alpha[           0][s] = alpha_mp[s];
		beta[beta.size()-1][s] =  beta_mp[s];
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::__save_alpha_beta()
{
	for (auto s = 0; s < n_states; s++)
	{
		alpha_mp[s] = alpha[alpha.size()-1][s];
		 beta_mp[s] = beta[0][s];
	}
}


template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::__fwd_recursion(const R *sys, const R *par)
{
	for (auto k = 0; k < this->K/2; k++)
	{
		for (auto s = 0; s < n_states; s++)
		{
			int s0 = trellis[1][s*4 + 0];
			int s1 = trellis[1][s*4 + 1];
			int s2 = trellis[1][s*4 + 2];
			int s3 = trellis[1][s*4 + 3];

			R y = par[2*k  ];
			R w = par[2*k+1];

			R p0 = trellis[2][s0*4+0]*y + trellis[3][s0*4+0]*w;
			R p1 = trellis[2][s1*4+1]*y + trellis[3][s1*4+1]*w;
			R p2 = trellis[2][s2*4+2]*y + trellis[3][s2*4+2]*w;
			R p3 = trellis[2][s3*4+3]*y + trellis[3][s3*4+3]*w;

			gamma[k][4*s0 + 0] = p0 + sys[4*k + 0];
			gamma[k][4*s1 + 1] = p1 + sys[4*k + 1];
			gamma[k][4*s2 + 2] = p2 + sys[4*k + 2];
			gamma[k][4*s3 + 3] = p3 + sys[4*k + 3];

			alpha[k+1][s] = MAX(MAX(alpha[k][s0] + gamma[k][4*s0 + 0],
			                        alpha[k][s1] + gamma[k][4*s1 + 1]),
			                    MAX(alpha[k][s2] + gamma[k][4*s2 + 2],
			                        alpha[k][s3] + gamma[k][4*s3 + 3]));
		}
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::__bwd_recursion(const R *sys, const R *par, R* ext)
{
	for (auto k = this->K/2 - 1; k >= 0; k--)
	{
		for (auto s = 0; s < n_states; s++)
		{
			beta[k][s] = MAX(MAX(beta[k+1][trellis[0][4*s + 0]] + gamma[k][4*s + 0],
			                     beta[k+1][trellis[0][4*s + 1]] + gamma[k][4*s + 1]),
			                 MAX(beta[k+1][trellis[0][4*s + 2]] + gamma[k][4*s + 2],
			                     beta[k+1][trellis[0][4*s + 3]] + gamma[k][4*s + 3]));
		}

		R norm = beta[k][0];
		for (auto s = 0; s < n_states; s++)
			beta[k][s] -= norm;

		R post0 = std::numeric_limits<R>::lowest();
		R post1 = std::numeric_limits<R>::lowest();
		R post2 = std::numeric_limits<R>::lowest();
		R post3 = std::numeric_limits<R>::lowest();

		for (auto s = 0; s < n_states; s++)
		{
			post0 = MAX(post0, alpha[k][s] + gamma[k][4*s + 0] + beta[k+1][trellis[0][4*s + 0]]);
			post1 = MAX(post1, alpha[k][s] + gamma[k][4*s + 1] + beta[k+1][trellis[0][4*s + 1]]);
			post2 = MAX(post2, alpha[k][s] + gamma[k][4*s + 2] + beta[k+1][trellis[0][4*s + 2]]);
			post3 = MAX(post3, alpha[k][s] + gamma[k][4*s + 3] + beta[k+1][trellis[0][4*s + 3]]);
		}

		ext[4*k + 0] = post0 - sys[4*k + 0];
		ext[4*k + 1] = post1 - sys[4*k + 1];
		ext[4*k + 2] = post2 - sys[4*k + 2];
		ext[4*k + 3] = post3 - sys[4*k + 3];
	}
}

template <typename B, typename R, tools::proto_max<R> MAX>
void Decoder_RSC_DB_BCJR<B,R,MAX>
::notify_new_frame()
{
	for (auto s = 0; s < n_states; s++)
	{
		alpha_mp[s] = (R)0;
		beta_mp [s] = (R)0;
	}
}
}
}
