#include <cassert>
#include <iomanip>
#include <type_traits>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Modem_SCMA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Modem_SCMA<B,R,Q,PSI>
::Modem_SCMA(const int N, const std::string& codebook_path, const tools::Noise<R>& noise, const int bps,
             const bool disable_sig2, const int n_ite, const int n_frames)
: Modem<B,R,Q>(N,
               Modem_SCMA<B,R,Q,PSI>::size_mod(N, bps),
               Modem_SCMA<B,R,Q,PSI>::size_fil(N, bps),
               noise,
               n_frames),
  CB                   (codebook_path),
  arr_phi              (CB.get_number_of_resources(), CB.get_codebook_size(),       CB.get_codebook_size(), CB.get_codebook_size(), (Q)0),
  msg_user_to_resources(CB.get_number_of_users(),     CB.get_number_of_resources(), CB.get_codebook_size(),                         (Q)0),
  msg_res_user         (CB.get_number_of_resources(), CB.get_number_of_users(),     CB.get_codebook_size(),                         (Q)0),
  guess                (CB.get_number_of_users(),     CB.get_codebook_size(),                                                       (Q)0),
  disable_sig2         (disable_sig2 ),
  n_ite                (n_ite        ),
  bps                  (bps          )
{
	const std::string name = "Modem_SCMA";
	this->set_name(name);

	if (n_frames != CB.get_number_of_users())
	{
		std::stringstream message;
		message << "'n_frames' has to be equal to CB.get_number_of_users() ('n_frames' = " << n_frames
		        << ", 'CB.get_number_of_users()' = " << CB.get_number_of_users() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (bps != 3) // TODO: With what should "3" be replaced ? -> have added "bps" attribute to the class, maybe you'll
	              //       need to use it in the code to have it generic
	{
		std::stringstream message;
		message << "'bps' has to be equal to 3 ('bps' = " << bps << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::set_noise(const tools::Noise<R>& noise)
{
	Modem<B,R,Q>::set_noise(noise);

	this->n->is_of_type_throw(tools::Noise_type::SIGMA);

	this->n0 = this->disable_sig2 ?
	            (R)1.0 :
	            ((R)4.0 * this->n->get_noise() * this->n->get_noise());
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::modulate(const B* X_N1, R* X_N2, const int frame_id)
{
	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto N_mod = 8 * ((this->N + 1) / 2); // TODO:  With what should "8" be replaced ?

	for (auto f = 0 ; f < this->n_frames ; f++)
	{
		for (auto j = 0 ; j < this->N / 2 ; j++)
		{
			unsigned idx = 0;

			for (unsigned i = 0 ; i < 2 ; i++) // TODO:  With what should "2" be replaced ?
				idx += (1 << i) * (unsigned)X_N1[f * this->N + 2 * j + i];

			for (auto i = 0 ; i < CB.get_number_of_resources() ; i++)
			{
				X_N2[f * N_mod + 8 * j + 2 * i   ] = CB(f, i, idx).real();
				X_N2[f * N_mod + 8 * j + 2 * i +1] = CB(f, i, idx).imag();
			}
		}
	}

	if (this->N % 2 == 1)
	{
		for (auto f = 0 ; f < this->n_frames ; f++)
		{
			unsigned idx = (unsigned)X_N1[f * this->N + this->N - 1];

			for (auto i = 0 ; i < CB.get_number_of_resources() ; i++)
			{
				X_N2[f * N_mod + 8 * (this->N / 2) + 2 * i   ] = CB(f, i, idx).real(); // TODO:  With what should "8" be replaced ?
				X_N2[f * N_mod + 8 * (this->N / 2) + 2 * i +1] = CB(f, i, idx).imag(); // TODO:  With what should "2" be replaced ?
			}
		}
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

	for (auto batch = 0 ; batch < (this->N +1) / 2 ; batch++)
	{
		// filling array_phi luts
		for (auto i = 0; i < CB.get_codebook_size(); i++)
			for (auto j = 0; j < CB.get_codebook_size(); j++)
				for (auto k = 0; k < CB.get_codebook_size(); k++)
					for (auto re = 0; re < CB.get_number_of_resources(); re++)
						arr_phi(re,i,j,k) = phi(Y_N1, i, j, k, re, batch, H_N);

		demodulate_batch(Y_N1,Y_N2,batch);
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

	for (auto batch = 0 ; batch < (this->N +1) / 2 ; batch++)
	{
		// filling array_phi luts
		for (auto i = 0; i < CB.get_codebook_size(); i++)
			for (auto j = 0; j < CB.get_codebook_size(); j++)
				for (auto k = 0; k < CB.get_codebook_size(); k++)
					for (auto re = 0; re < CB.get_number_of_resources(); re++)
						arr_phi(re,i,j,k) = phi(Y_N1, i, j, k, re, batch);

		demodulate_batch(Y_N1,Y_N2,batch);
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate_batch(const Q* Y_N1, Q* Y_N2, int batch)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

	// initial probability of each codeword/user
	auto init_proba = (Q)1. / (Q)CB.get_codebook_size();
	for (auto u = 0; u < CB.get_number_of_users(); ++u)
		for (auto r = 0; r < CB.get_number_of_resources_per_user(); ++r)
			for (auto i = 0; i < CB.get_codebook_size(); i++)
				msg_user_to_resources(u,CB.get_user_to_resource(u,r),i) = init_proba;

	// starting iteration
	for (auto itrr = 0; itrr < n_ite; itrr++)
	{
		// resource to user messaging
		// initialization
		for (auto r = 0; r < CB.get_number_of_resources(); ++r)
			for (auto u = 0; u < CB.get_number_of_users_per_resource(); ++u)
				for (auto i = 0; i < CB.get_codebook_size(); i++)
					msg_res_user(r,CB.get_resource_to_user(r,u),i) = (Q)0;


		for (auto i = 0; i < CB.get_codebook_size(); i++)
			for (auto j = 0; j < CB.get_codebook_size(); j++)
				for (auto k = 0; k < CB.get_codebook_size(); k++)
					for (auto re = 0; re < CB.get_number_of_resources(); re++)
					{
						msg_res_user(re,CB.get_resource_to_user(re,0),i) += arr_phi(re,i,j,k)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re,1),re,j)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re,2),re,k);
						msg_res_user(re,CB.get_resource_to_user(re,1),i) += arr_phi(re,j,i,k)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re,0),re,j)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re,2),re,k);
						msg_res_user(re,CB.get_resource_to_user(re,2),i) += arr_phi(re,j,k,i)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re,0),re,j)
						                                            * msg_user_to_resources(CB.get_resource_to_user(re,1),re,k);
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

		// 					msg_res_user(re,CB.get_resource_to_user(re,u),i) += proba;
		// 				}


		// user to resource messaging
		for (auto i = 0; i < CB.get_number_of_resources(); i++)
		{
			msg_user_to_resources(0,CB.get_user_to_resource(0,0),i) = msg_res_user(CB.get_user_to_resource(0,1),0,i) /
													 (msg_res_user(CB.get_user_to_resource(0,1),0,0) +
													  msg_res_user(CB.get_user_to_resource(0,1),0,1) +
			                                          msg_res_user(CB.get_user_to_resource(0,1),0,2) +
													  msg_res_user(CB.get_user_to_resource(0,1),0,3));
			msg_user_to_resources(0,CB.get_user_to_resource(0,1),i) = msg_res_user(CB.get_user_to_resource(0,0),0,i) /
													 (msg_res_user(CB.get_user_to_resource(0,0),0,0) +
													  msg_res_user(CB.get_user_to_resource(0,0),0,1) +
													  msg_res_user(CB.get_user_to_resource(0,0),0,2) +
													  msg_res_user(CB.get_user_to_resource(0,0),0,3));
			msg_user_to_resources(1,CB.get_user_to_resource(1,0),i) = msg_res_user(CB.get_user_to_resource(1,1),1,i) /
													 (msg_res_user(CB.get_user_to_resource(1,1),1,0) +
													  msg_res_user(CB.get_user_to_resource(1,1),1,1) +
													  msg_res_user(CB.get_user_to_resource(1,1),1,2) +
													  msg_res_user(CB.get_user_to_resource(1,1),1,3));
			msg_user_to_resources(1,CB.get_user_to_resource(1,1),i) = msg_res_user(CB.get_user_to_resource(1,0),1,i) /
													 (msg_res_user(CB.get_user_to_resource(1,0),1,0) +
													  msg_res_user(CB.get_user_to_resource(1,0),1,1) +
													  msg_res_user(CB.get_user_to_resource(1,0),1,2) +
													  msg_res_user(CB.get_user_to_resource(1,0),1,3));
			msg_user_to_resources(2,CB.get_user_to_resource(2,0),i) = msg_res_user(CB.get_user_to_resource(2,1),2,i) /
													 (msg_res_user(CB.get_user_to_resource(2,1),2,0) +
													  msg_res_user(CB.get_user_to_resource(2,1),2,1) +
													  msg_res_user(CB.get_user_to_resource(2,1),2,2) +
													  msg_res_user(CB.get_user_to_resource(2,1),2,3));
			msg_user_to_resources(2,CB.get_user_to_resource(2,1),i) = msg_res_user(CB.get_user_to_resource(2,0),2,i) /
													 (msg_res_user(CB.get_user_to_resource(2,0),2,0) +
													  msg_res_user(CB.get_user_to_resource(2,0),2,1) +
													  msg_res_user(CB.get_user_to_resource(2,0),2,2) +
													  msg_res_user(CB.get_user_to_resource(2,0),2,3));
			msg_user_to_resources(3,CB.get_user_to_resource(3,0),i) = msg_res_user(CB.get_user_to_resource(3,1),3,i) /
													 (msg_res_user(CB.get_user_to_resource(3,1),3,0) +
													  msg_res_user(CB.get_user_to_resource(3,1),3,1) +
													  msg_res_user(CB.get_user_to_resource(3,1),3,2) +
													  msg_res_user(CB.get_user_to_resource(3,1),3,3));
			msg_user_to_resources(3,CB.get_user_to_resource(3,1),i) = msg_res_user(CB.get_user_to_resource(3,0),3,i) /
													 (msg_res_user(CB.get_user_to_resource(3,0),3,0) +
													  msg_res_user(CB.get_user_to_resource(3,0),3,1) +
													  msg_res_user(CB.get_user_to_resource(3,0),3,2) +
													  msg_res_user(CB.get_user_to_resource(3,0),3,3));
			msg_user_to_resources(4,CB.get_user_to_resource(4,0),i) = msg_res_user(CB.get_user_to_resource(4,1),4,i) /
													 (msg_res_user(CB.get_user_to_resource(4,1),4,0) +
													  msg_res_user(CB.get_user_to_resource(4,1),4,1) +
													  msg_res_user(CB.get_user_to_resource(4,1),4,2) +
													  msg_res_user(CB.get_user_to_resource(4,1),4,3));
			msg_user_to_resources(4,CB.get_user_to_resource(4,1),i) = msg_res_user(CB.get_user_to_resource(4,0),4,i) /
													 (msg_res_user(CB.get_user_to_resource(4,0),4,0) +
													  msg_res_user(CB.get_user_to_resource(4,0),4,1) +
													  msg_res_user(CB.get_user_to_resource(4,0),4,2) +
													  msg_res_user(CB.get_user_to_resource(4,0),4,3));
			msg_user_to_resources(5,CB.get_user_to_resource(5,0),i) = msg_res_user(CB.get_user_to_resource(5,1),5,i) /
													 (msg_res_user(CB.get_user_to_resource(5,1),5,0) +
													  msg_res_user(CB.get_user_to_resource(5,1),5,1) +
													  msg_res_user(CB.get_user_to_resource(5,1),5,2) +
													  msg_res_user(CB.get_user_to_resource(5,1),5,3));
			msg_user_to_resources(5,CB.get_user_to_resource(5,1),i) = msg_res_user(CB.get_user_to_resource(5,0),5,i) /
													 (msg_res_user(CB.get_user_to_resource(5,0),5,0) +
													  msg_res_user(CB.get_user_to_resource(5,0),5,1) +
													  msg_res_user(CB.get_user_to_resource(5,0),5,2) +
													  msg_res_user(CB.get_user_to_resource(5,0),5,3));
		}
	}
	// end of iterations

	// guess at each user
	for (auto i = 0; i < CB.get_codebook_size(); i++) //codeword index
	{
		guess(0,i) = msg_res_user(CB.get_user_to_resource(0,0),0,i) * msg_res_user(CB.get_user_to_resource(0,1),0,i);
		guess(1,i) = msg_res_user(CB.get_user_to_resource(1,0),1,i) * msg_res_user(CB.get_user_to_resource(1,1),1,i);
		guess(2,i) = msg_res_user(CB.get_user_to_resource(2,0),2,i) * msg_res_user(CB.get_user_to_resource(2,1),2,i);
		guess(3,i) = msg_res_user(CB.get_user_to_resource(3,0),3,i) * msg_res_user(CB.get_user_to_resource(3,1),3,i);
		guess(4,i) = msg_res_user(CB.get_user_to_resource(4,0),4,i) * msg_res_user(CB.get_user_to_resource(4,1),4,i);
		guess(5,i) = msg_res_user(CB.get_user_to_resource(5,0),5,i) * msg_res_user(CB.get_user_to_resource(5,1),5,i);
	}

	// LLRs computation
	Y_N2[0 * this->N + batch *2 +0] = (Q)(std::log(guess(0,0) + guess(0,2)) - std::log(guess(0,1) + guess(0,3)));
	Y_N2[1 * this->N + batch *2 +0] = (Q)(std::log(guess(1,0) + guess(1,2)) - std::log(guess(1,1) + guess(1,3)));
	Y_N2[2 * this->N + batch *2 +0] = (Q)(std::log(guess(2,0) + guess(2,2)) - std::log(guess(2,1) + guess(2,3)));
	Y_N2[3 * this->N + batch *2 +0] = (Q)(std::log(guess(3,0) + guess(3,2)) - std::log(guess(3,1) + guess(3,3)));
	Y_N2[4 * this->N + batch *2 +0] = (Q)(std::log(guess(4,0) + guess(4,2)) - std::log(guess(4,1) + guess(4,3)));
	Y_N2[5 * this->N + batch *2 +0] = (Q)(std::log(guess(5,0) + guess(5,2)) - std::log(guess(5,1) + guess(5,3)));

	if((this->N % 2) != 1 || batch != ((this->N /2 +1) -1))
	{
		Y_N2[0 * this->N + batch *2 +1] = (Q)(std::log(guess(0,0) + guess(0,1)) - std::log(guess(0,2) + guess(0,3)));
		Y_N2[1 * this->N + batch *2 +1] = (Q)(std::log(guess(1,0) + guess(1,1)) - std::log(guess(1,2) + guess(1,3)));
		Y_N2[2 * this->N + batch *2 +1] = (Q)(std::log(guess(2,0) + guess(2,1)) - std::log(guess(2,2) + guess(2,3)));
		Y_N2[3 * this->N + batch *2 +1] = (Q)(std::log(guess(3,0) + guess(3,1)) - std::log(guess(3,2) + guess(3,3)));
		Y_N2[4 * this->N + batch *2 +1] = (Q)(std::log(guess(4,0) + guess(4,1)) - std::log(guess(4,2) + guess(4,3)));
		Y_N2[5 * this->N + batch *2 +1] = (Q)(std::log(guess(5,0) + guess(5,1)) - std::log(guess(5,2) + guess(5,3)));
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Q Modem_SCMA<B,R,Q,PSI>
::phi(const Q* Y_N1, int i, int j, int k, int re, int batch)
{
	Q phi;
	std::complex<Q> tmp;

	auto Y_N = std::complex<Q>(Y_N1[batch *8 + 2*re], Y_N1[batch*8 + 2*re +1]);

	const auto CB0 = std::complex<Q>((Q)CB(CB.get_resource_to_user(re,0), re, i).real(), (Q)CB(CB.get_resource_to_user(re,0), re, i).imag());
	const auto CB1 = std::complex<Q>((Q)CB(CB.get_resource_to_user(re,1), re, j).real(), (Q)CB(CB.get_resource_to_user(re,1), re, j).imag());
	const auto CB2 = std::complex<Q>((Q)CB(CB.get_resource_to_user(re,2), re, k).real(), (Q)CB(CB.get_resource_to_user(re,2), re, k).imag());

	tmp = Y_N - (CB0 + CB1 + CB2);

	phi = PSI(tmp, (Q)n0);

	return phi;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Q Modem_SCMA<B,R,Q,PSI>
::phi(const Q* Y_N1, int i, int j, int k, int re, int batch, const R* H_N)
{
	Q phi;
	std::complex<Q> tmp;
	const auto Nmod = size_mod(this->N, 3);

	const auto Y_N  = std::complex<Q>(Y_N1[batch *8 + 2*re], Y_N1[batch*8 + 2*re +1]);

	const auto H_N0 = std::complex<Q>((Q)H_N[CB.get_resource_to_user(re,0) * Nmod + 8 * batch + 2 * re   ],
	                                  (Q)H_N[CB.get_resource_to_user(re,0) * Nmod + 8 * batch + 2 * re +1]);
	const auto H_N1 = std::complex<Q>((Q)H_N[CB.get_resource_to_user(re,1) * Nmod + 8 * batch + 2 * re   ],
	                                  (Q)H_N[CB.get_resource_to_user(re,1) * Nmod + 8 * batch + 2 * re +1]);
	const auto H_N2 = std::complex<Q>((Q)H_N[CB.get_resource_to_user(re,2) * Nmod + 8 * batch + 2 * re   ],
	                                  (Q)H_N[CB.get_resource_to_user(re,2) * Nmod + 8 * batch + 2 * re +1]);

	const auto CB0  = std::complex<Q>((Q)CB(CB.get_resource_to_user(re,0), re, i).real(), (Q)CB(CB.get_resource_to_user(re,0), re, i).imag());
	const auto CB1  = std::complex<Q>((Q)CB(CB.get_resource_to_user(re,1), re, j).real(), (Q)CB(CB.get_resource_to_user(re,1), re, j).imag());
	const auto CB2  = std::complex<Q>((Q)CB(CB.get_resource_to_user(re,2), re, k).real(), (Q)CB(CB.get_resource_to_user(re,2), re, k).imag());

	tmp = Y_N - (H_N0 * CB0 + H_N1 * CB1 + H_N2 * CB2);

	phi = PSI(tmp, (Q)n0);

	return phi;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::copy(Y_N1, Y_N1 + this->N_fil + this->n_frames, Y_N2);
}
}
}
