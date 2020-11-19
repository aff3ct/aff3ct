#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>
#include <complex>
#include <algorithm>
#include <type_traits>
#include <sstream>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Modem/SCMA/Modem_SCMA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Modem_SCMA<B,R,Q,PSI>
::Modem_SCMA(const int N, const std::string &codebook_path, const bool disable_sig2, const int n_ite)
: Modem<B,R,Q>(N,
               Modem_SCMA<B,R,Q,PSI>::size_mod(N, codebook_path),
               Modem_SCMA<B,R,Q,PSI>::size_fil(N, codebook_path)),
  CB                   (codebook_path),
  arr_phi              (CB.get_number_of_resources(), CB.get_codebook_size(),       CB.get_codebook_size(), CB.get_codebook_size(), (Q)0),
  msg_user_to_resources(CB.get_number_of_users(),     CB.get_number_of_resources(), CB.get_codebook_size(),                         (Q)0),
  msg_resource_to_users(CB.get_number_of_resources(), CB.get_number_of_users(),     CB.get_codebook_size(),                         (Q)0),
  guess                (CB.get_number_of_users(),     CB.get_codebook_size(),                                                       (Q)0),
  disable_sig2         (disable_sig2 ),
  n0                   ((R)1.0       ),
  n_ite                (n_ite        )
{
	const std::string name = "Modem_SCMA";
	this->set_name(name);
	this->set_n_frames((size_t)CB.get_number_of_users());
	this->set_single_wave(true);

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Modem_SCMA<B,R,Q,PSI>* Modem_SCMA<B,R,Q,PSI>
::clone() const
{
	auto m = new Modem_SCMA(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::_modulate(const B* X_N1, R* X_N2, const size_t frame_id)
{
	const auto N_mod = CB.get_number_of_real_symbols() * ((this->N + 1) / 2);

	for (size_t f = 0 ; f < this->n_frames ; f++)
	{
		for (auto j = 0 ; j < this->N / 2 ; j++)
		{
			unsigned idx = 0;

			for (unsigned i = 0 ; i < 2 ; i++) // TODO:  With what should "2" be replaced ?
				idx += (1 << i) * (unsigned)X_N1[f * this->N + 2 * j + i];

			for (auto i = 0 ; i < CB.get_number_of_resources() ; i++)
			{
				X_N2[f * N_mod + CB.get_number_of_real_symbols() * j + 2 * i    ] = CB(f, i, idx).real();
				X_N2[f * N_mod + CB.get_number_of_real_symbols() * j + 2 * i + 1] = CB(f, i, idx).imag();
			}
		}
	}

	if (this->N % 2 == 1)
	{
		for (size_t f = 0 ; f < this->n_frames ; f++)
		{
			unsigned idx = (unsigned)X_N1[f * this->N + this->N - 1];

			for (auto i = 0 ; i < CB.get_number_of_resources() ; i++)
			{
				X_N2[f * N_mod + CB.get_number_of_real_symbols() * (this->N / 2) + 2 * i    ] = CB(f, i, idx).real();
				X_N2[f * N_mod + CB.get_number_of_real_symbols() * (this->N / 2) + 2 * i + 1] = CB(f, i, idx).imag();
			}
		}
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::_demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (*CP != this->last_channel_param && !this->disable_sig2)
		this->n0 = ((R)4.0 * *CP * *CP);

	for (auto batch = 0 ; batch < (this->N +1) / 2 ; batch++)
	{
		// filling array_phi luts
		for (auto i = 0; i < CB.get_codebook_size(); i++)
			for (auto j = 0; j < CB.get_codebook_size(); j++)
				for (auto k = 0; k < CB.get_codebook_size(); k++)
					for (auto re = 0; re < CB.get_number_of_resources(); re++)
						arr_phi(re, i, j, k) = phi(Y_N1, i, j, k, re, batch, H_N);

		demodulate_batch(Y_N2, batch);
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (*CP != this->last_channel_param && !this->disable_sig2)
		this->n0 = ((R)4.0 * *CP * *CP);

	for (auto batch = 0 ; batch < (this->N +1) / 2 ; batch++)
	{
		// filling array_phi luts
		for (auto i = 0; i < CB.get_codebook_size(); i++)
			for (auto j = 0; j < CB.get_codebook_size(); j++)
				for (auto k = 0; k < CB.get_codebook_size(); k++)
					for (auto re = 0; re < CB.get_number_of_resources(); re++)
						arr_phi(re, i, j, k) = phi(Y_N1, i, j, k, re, batch);

		demodulate_batch(Y_N2, batch);
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate_batch(Q* Y_N2, int batch)
{
	// initial probability of each codeword/user
	auto init_proba = (Q)1. / (Q)CB.get_codebook_size();
	for (auto u = 0; u < CB.get_number_of_users(); ++u)
		for (auto r = 0; r < CB.get_number_of_resources_per_user(); ++r)
			for (auto i = 0; i < CB.get_codebook_size(); i++)
				msg_user_to_resources(u, CB.get_user_to_resource(u, r), i) = init_proba;

	// starting iteration
	for (auto itrr = 0; itrr < n_ite; itrr++)
	{
		// resource to user messaging
		// initialization
		for (auto r = 0; r < CB.get_number_of_resources(); ++r)
			for (auto u = 0; u < CB.get_number_of_users_per_resource(); ++u)
				for (auto i = 0; i < CB.get_codebook_size(); i++)
					msg_resource_to_users(r, CB.get_resource_to_user(r, u), i) = (Q)0;


		for (auto i = 0; i < CB.get_codebook_size(); i++)
			for (auto j = 0; j < CB.get_codebook_size(); j++)
				for (auto k = 0; k < CB.get_codebook_size(); k++)
					for (auto re = 0; re < CB.get_number_of_resources(); re++)
					{
						msg_resource_to_users(re, CB.get_resource_to_user(re, 0), i) += arr_phi(re, i, j, k)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re, 1), re, j)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re, 2), re, k);
						msg_resource_to_users(re, CB.get_resource_to_user(re, 1), i) += arr_phi(re, j, i, k)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re, 0), re, j)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re, 2), re, k);
						msg_resource_to_users(re, CB.get_resource_to_user(re, 2), i) += arr_phi(re, j, k, i)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re, 0), re, j)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re, 1), re, k);
					}

		/// I tried to convert this last quadruple loops into this:
		// for (auto i = 0; i < CB.get_codebook_size(); i++)
		// 	for (auto j = 0; j < CB.get_codebook_size(); j++)
		// 		for (auto k = 0; k < CB.get_codebook_size(); k++)
		// 			for (auto re = 0; re < CB.get_number_of_resources(); re++)
		// 				for (auto u = 0; u < CB.get_number_of_users_per_resource(); u++)
		// 				{
		// 					auto proba = arr_phi(re,i,j,k);

		// 					for (auto u2 = 0; u2 < CB.get_number_of_users_per_resource(); u2++)
		// 						if (u2 != u)
		// 							proba *= msg_user_to_resources(CB.get_resource_to_user(re,u2),re,j); ///// <------- but I'm having issue with this j that shall be k once on two.

		// 					msg_resource_to_users(re,CB.get_resource_to_user(re,u),i) += proba;
		// 				}


		// user to resource messaging
		for (auto i = 0; i < CB.get_codebook_size(); i++) //codeword index
		{
			for (auto j = 0; j < CB.get_number_of_users(); j++) //user index
			{
				msg_user_to_resources(j, CB.get_user_to_resource(j, 0), i) = normalize_prob_msg_resource_to_users(j, 1, i);
				msg_user_to_resources(j, CB.get_user_to_resource(j, 1), i) = normalize_prob_msg_resource_to_users(j, 0, i);
			}
		}
	}
	// end of iterations

	// guess at each user
	for (auto i = 0; i < CB.get_codebook_size(); i++) //codeword index
	{
		for (auto j = 0; j < CB.get_number_of_users(); j++) //user index
		{
			// TODO : need certainly a third loop here for the different resources of a the user 'j'
			guess(j, i) = msg_resource_to_users(CB.get_user_to_resource(j, 0), j, i) * msg_resource_to_users(CB.get_user_to_resource(j, 1), j, i);
		}
	}

	// LLRs computation
	for (auto i = 0; i < CB.get_number_of_users(); i++) //user index
	{
		Y_N2[i * this->N + batch * 2 +0] = (Q)(std::log(guess(i, 0) + guess(i, 2)) - std::log(guess(i, 1) + guess(i, 3)));
	}

	if((this->N % 2) != 1 || batch != ((this->N /2 +1) -1))
	{
		for (auto i = 0; i < CB.get_number_of_users(); i++) //user index
		{
			Y_N2[i * this->N + batch * 2 +1] = (Q)(std::log(guess(i, 0) + guess(i, 1)) - std::log(guess(i, 2) + guess(i, 3)));
		}
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Q Modem_SCMA<B,R,Q,PSI>
::phi(const Q* Y_N1, int i, int j, int k, int re, int batch)
{
	auto Y_N = std::complex<Q>(Y_N1[batch * CB.get_number_of_real_symbols() + 2 * re],
                               Y_N1[batch * CB.get_number_of_real_symbols() + 2 * re + 1]);

	const auto CB0 = std::complex<Q>(CB(CB.get_resource_to_user(re, 0), re, i)); // convert from R to Q
	const auto CB1 = std::complex<Q>(CB(CB.get_resource_to_user(re, 1), re, j));
	const auto CB2 = std::complex<Q>(CB(CB.get_resource_to_user(re, 2), re, k));

	auto tmp = Y_N - (CB0 + CB1 + CB2);
	auto phi = PSI(tmp, (Q)n0);

	return phi;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Q Modem_SCMA<B,R,Q,PSI>
::normalize_prob_msg_resource_to_users(int user, int resource_ind, int codeword)
{
	Q sum = 0;

	for(auto i = 0; i < CB.get_codebook_size(); i++) //codeword index
	{
		sum += msg_resource_to_users(CB.get_user_to_resource(user, resource_ind), user, i);
	}

	auto norm_prob = msg_resource_to_users(CB.get_user_to_resource(user, resource_ind), user, codeword) / sum;
	return norm_prob;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Q Modem_SCMA<B,R,Q,PSI>
::phi(const Q* Y_N1, int i, int j, int k, int re, int batch, const R* H_N)
{
	Q phi;
	std::complex<Q> tmp;
	const auto Nmod = size_mod(this->N, 3);

	const auto Y_N  = std::complex<Q>(Y_N1[batch * CB.get_number_of_real_symbols() + 2 * re],
                                      Y_N1[batch * CB.get_number_of_real_symbols() + 2 * re + 1]);

	const auto H_N0 = std::complex<Q>((Q)H_N[CB.get_resource_to_user(re, 0) * Nmod
                                           + CB.get_number_of_real_symbols() * batch + 2 * re   ],
	                                  (Q)H_N[CB.get_resource_to_user(re, 0) * Nmod
                                           + CB.get_number_of_real_symbols() * batch + 2 * re + 1]);
	const auto H_N1 = std::complex<Q>((Q)H_N[CB.get_resource_to_user(re, 1) * Nmod
                                           + CB.get_number_of_real_symbols() * batch + 2 * re   ],
	                                  (Q)H_N[CB.get_resource_to_user(re, 1) * Nmod
                                           + CB.get_number_of_real_symbols() * batch + 2 * re + 1]);
	const auto H_N2 = std::complex<Q>((Q)H_N[CB.get_resource_to_user(re, 2) * Nmod
                                           + CB.get_number_of_real_symbols() * batch + 2 * re   ],
	                                  (Q)H_N[CB.get_resource_to_user(re, 2) * Nmod
                                           + CB.get_number_of_real_symbols() * batch + 2 * re + 1]);

	const auto CB0  = std::complex<Q>(CB(CB.get_resource_to_user(re, 0), re, i));
	const auto CB1  = std::complex<Q>(CB(CB.get_resource_to_user(re, 1), re, j));
	const auto CB2  = std::complex<Q>(CB(CB.get_resource_to_user(re, 2), re, k));

	tmp = Y_N - (H_N0 * CB0 + H_N1 * CB1 + H_N2 * CB2);

	phi = PSI(tmp, (Q)n0);

	return phi;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::_filter(const float *noise, const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil * this->n_frames, Y_N2);
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
bool Modem_SCMA<B,R,Q,PSI>
::is_complex_mod()
{
	return true;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
bool Modem_SCMA<B,R,Q,PSI>
::is_complex_fil()
{
	return true;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
int Modem_SCMA<B,R,Q,PSI>
::size_mod(const int N, const int bps)
{
	return ((int)std::pow(2, bps) * ((N + 1) / 2));
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
int Modem_SCMA<B,R,Q,PSI>
::size_fil(const int N, const int bps)
{
	return size_mod(N, bps);
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
int Modem_SCMA<B,R,Q,PSI>
::size_mod(const int N, const std::string &codebook_path)
{
	const tools::Codebook<R> CB(codebook_path);
	int bps = (int)CB.get_system_bps();
	return ((int)std::pow(2, bps) * ((N + 1) / 2));
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
int Modem_SCMA<B,R,Q,PSI>
::size_fil(const int N, const std::string &codebook_path)
{
	const tools::Codebook<R> CB(codebook_path);
	int bps = (int)CB.get_system_bps();
	return size_mod(N, bps);
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::set_n_frames(const size_t n_frames)
{
	if (n_frames != (size_t)CB.get_number_of_users())
	{
		std::stringstream message;
		message << "'n_frames' has to be equal to CB.get_number_of_users() ('n_frames' = " << n_frames
		        << ", 'CB.get_number_of_users()' = " << CB.get_number_of_users() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
		Modem<B,R,Q>::set_n_frames(n_frames);
}

}
}
