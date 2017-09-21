#include <cassert>
#include <iomanip>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Modem_SCMA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
const std::complex<float> Modem_SCMA<B,R,Q,PSI>::CB[6][4][4] =
{
	{ // codebook1 (code layer 1)
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ { -0.1815f, -0.1318f }, { -0.6351f, -0.4615f }, {  0.6351f,  0.4615f }, {  0.1815f,  0.1318f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ {  0.7851f,  0.0000f }, { -0.2243f,  0.0000f }, {  0.2243f,  0.0000f }, { -0.7851f,  0.0000f } }
	},
	{ // codebook2 (code layer 2)
		{ {  0.7851f,  0.0000f }, { -0.2243f,  0.0000f }, {  0.2243f,  0.0000f }, { -0.7851f,  0.0000f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ { -0.1815f, -0.1318f }, { -0.6351f, -0.4615f }, {  0.6351f,  0.4615f }, {  0.1815f,  0.1318f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } }
	},
	{ // codebook3 (code layer 3)
		{ { -0.6351f,  0.4615f }, {  0.1815f, -0.1318f }, { -0.1815f,  0.1318f }, {  0.6351f, -0.4615f } },
		{ {  0.1392f, -0.1759f }, {  0.4873f, -0.6156f }, { -0.4873f,  0.6156f }, { -0.1392f,  0.1759f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ {  0.0000f,  0.0000f }, {  00000.f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } }
	},
	{ // codebook4 (code layer 4)
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ {  0.7851f,  0.0000f }, { -0.2243f,  0.0000f }, {  0.2243f,  0.0000f }, { -0.7851f,  0.0000f } },
		{ { -0.0055f, -0.2242f }, { -0.0193f, -0.7848f }, {  0.0193f,  0.7848f }, {  0.0055f,  0.2242f } }
	},
	{ // codebook5 (code layer 5)
		{ { -0.0055f, -0.2242f }, { -0.0193f, -0.7848f }, {  0.0193f,  0.7848f }, {  0.0055f,  0.2242f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ { -0.6351f,  0.4615f }, {  0.1815f, -0.1318f }, { -0.1815f,  0.1318f }, {  0.6351f, -0.4615f } }
	},
	{ // codebook6 (code layer 6)
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } },
		{ {  0.7851f,  0.0000f }, { -0.2243f,  0.0000f }, {  0.2243f,  0.0000f }, { -0.7851f,  0.0000f } },
		{ {  0.1392f, -0.1759f }, {  0.4873f, -0.6156f }, { -0.4873f,  0.6156f }, { -0.1392f,  0.1759f } },
		{ {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f }, {  0.0000f,  0.0000f } }
	},
};

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Modem_SCMA<B,R,Q,PSI>
::Modem_SCMA(const int N, const R sigma, const int bps, const bool disable_sig2, const int n_ite,
             const int n_frames, const std::string name)
: Modem<B,R,Q>(N,
               Modem_SCMA<B,R,Q,PSI>::size_mod(N, bps),
               Modem_SCMA<B,R,Q,PSI>::size_fil(N, bps),
               sigma,
               n_frames,
               name),
  disable_sig2(disable_sig2                                    ),
  n0          (disable_sig2 ? (R)1.0 : (R)2.0 *  sigma * sigma ),
  n_ite       (n_ite                                           )
{
	if (n_frames != 6)
	{
		std::stringstream message;
		message << "'n_frames' has to be equal to 6 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (bps != 3)
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
Modem_SCMA<B,R,Q,PSI>
::~Modem_SCMA()
{
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::set_sigma(const R sigma)
{
	Modem<B,R,Q>::set_sigma(sigma);

	if (!disable_sig2)
		this->n0 = (R)2.0 * sigma * sigma;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::modulate(const B* X_N1, R* X_N2)
{
	const auto N_mod = 8 * ((this->N + 1) / 2);

	for (auto f = 0 ; f < this->n_frames ; f++)
	{
		for (auto j = 0 ; j < this->N / 2 ; j++)
		{
			unsigned idx = 0;

			for (unsigned i = 0 ; i < 2 ; i++)
				idx += (1 << i) * (unsigned)X_N1[f * this->N + 2 * j + i];

			for (auto i = 0 ; i < 4 ; i++)
			{
				X_N2[f * N_mod + 8 * j + 2 * i   ] = CB[f][i][idx].real();
				X_N2[f * N_mod + 8 * j + 2 * i +1] = CB[f][i][idx].imag();
			}
		}
	}

	if (this->N % 2 == 1)
	{
		for (auto f = 0 ; f < this->n_frames ; f++)
		{
			unsigned idx = (unsigned)X_N1[f * this->N + this->N - 1];

			for (auto i = 0 ; i < 4 ; i++)
			{
				X_N2[f * N_mod + 8 * (this->N / 2) + 2 * i   ] = CB[f][i][idx].real();
				X_N2[f * N_mod + 8 * (this->N / 2) + 2 * i +1] = CB[f][i][idx].imag();
			}
		}
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate_wg(const Q *Y_N1, const R *H_N, Q *Y_N2)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	for (auto batch = 0 ; batch < (this->N +1) / 2 ; batch++)
	{
		// filling array_phi luts
		for (auto i = 0; i < 4; i++)
			for (auto j = 0; j < 4; j++)
				for (auto k = 0; k < 4; k++)
					for (auto re = 0; re < 4; re++)
						arr_phi[re][i][j][k] = phi(Y_N1, i, j, k, re, batch, H_N);

		demodulate_batch(Y_N1,Y_N2,batch);
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate(const Q *Y_N1, Q *Y_N2)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	for (auto batch = 0 ; batch < (this->N +1) / 2 ; batch++)
	{
		// filling array_phi luts
		for (auto i = 0; i < 4; i++)
			for (auto j = 0; j < 4; j++)
				for (auto k = 0; k < 4; k++)
					for (auto re = 0; re < 4; re++)
						arr_phi[re][i][j][k] = phi(Y_N1, i, j, k, re, batch);

		demodulate_batch(Y_N1,Y_N2,batch);
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::demodulate_batch(const Q* Y_N1, Q* Y_N2, int batch)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	// declarations
	Q msg_user_res[6][4][4] = {};
	Q msg_res_user[4][6][4] = {};
	Q guess[6][4] = {};

	// initial probability of each codeword/user
	for (auto i = 0; i < 4; i++)
	{
		msg_user_res[0][1][i] = (Q)0.25;
		msg_user_res[0][3][i] = (Q)0.25;
		msg_user_res[1][0][i] = (Q)0.25;
		msg_user_res[1][2][i] = (Q)0.25;
		msg_user_res[2][0][i] = (Q)0.25;
		msg_user_res[2][1][i] = (Q)0.25;
		msg_user_res[3][2][i] = (Q)0.25;
		msg_user_res[3][3][i] = (Q)0.25;
		msg_user_res[4][0][i] = (Q)0.25;
		msg_user_res[4][3][i] = (Q)0.25;
		msg_user_res[5][1][i] = (Q)0.25;
		msg_user_res[5][2][i] = (Q)0.25;
	}

	// starting iteration
	for (auto itrr = 0; itrr < n_ite; itrr++)
	{
		// resource to user messaging
		// initialization
		for (auto i = 0; i < 4; i++)
		{
			msg_res_user[0][1][i] = (Q)0;
			msg_res_user[0][2][i] = (Q)0;
			msg_res_user[0][4][i] = (Q)0;
			msg_res_user[1][0][i] = (Q)0;
			msg_res_user[1][2][i] = (Q)0;
			msg_res_user[1][5][i] = (Q)0;
			msg_res_user[2][1][i] = (Q)0;
			msg_res_user[2][3][i] = (Q)0;
			msg_res_user[2][5][i] = (Q)0;
			msg_res_user[3][0][i] = (Q)0;
			msg_res_user[3][3][i] = (Q)0;
			msg_res_user[3][4][i] = (Q)0;
		}

		for (auto i = 0; i < 4; i++)  // codeword index
			for (auto j = 0; j < 4; j++)
				for (auto k = 0; k < 4; k++)
					for(auto re = 0 ; re < 4 ; re++)
					{
						msg_res_user[re][re_user[re][0]][i] = msg_res_user[re][re_user[re][0]][i] 
						                                      +  arr_phi[re][i][j][k] 
						                                      * msg_user_res[re_user[re][1]][re][j] 
						                                      * msg_user_res[re_user[re][2]][re][k];
						msg_res_user[re][re_user[re][1]][i] = msg_res_user[re][re_user[re][1]][i] 
						                                      +  arr_phi[re][j][i][k] 
						                                      * msg_user_res[re_user[re][0]][re][j] 
						                                      * msg_user_res[re_user[re][2]][re][k];
						msg_res_user[re][re_user[re][2]][i] = msg_res_user[re][re_user[re][2]][i] 
						                                      +  arr_phi[re][j][k][i] 
						                                      * msg_user_res[re_user[re][0]][re][j] 
						                                      * msg_user_res[re_user[re][1]][re][k];
					}

		// user to resource messaging
		for (auto i = 0 ; i < 4 ; i++)
		{
			msg_user_res[0][1][i] = msg_res_user[3][0][i] / (msg_res_user[3][0][0] + msg_res_user[3][0][1] +
			                                                 msg_res_user[3][0][2] + msg_res_user[3][0][3]);
			msg_user_res[0][3][i] = msg_res_user[1][0][i] / (msg_res_user[1][0][0] + msg_res_user[1][0][1] +
			                                                 msg_res_user[1][0][2] + msg_res_user[1][0][3]);
			msg_user_res[1][0][i] = msg_res_user[2][1][i] / (msg_res_user[2][1][0] + msg_res_user[2][1][1] +
			                                                 msg_res_user[2][1][2] + msg_res_user[2][1][3]);
			msg_user_res[1][2][i] = msg_res_user[0][1][i] / (msg_res_user[0][1][0] + msg_res_user[0][1][1] +
			                                                 msg_res_user[0][1][2] + msg_res_user[0][1][3]);
			msg_user_res[2][0][i] = msg_res_user[1][2][i] / (msg_res_user[1][2][0] + msg_res_user[1][2][1] +
			                                                 msg_res_user[1][2][2] + msg_res_user[1][2][3]);
			msg_user_res[2][1][i] = msg_res_user[0][2][i] / (msg_res_user[0][2][0] + msg_res_user[0][2][1] +
			                                                 msg_res_user[0][2][2] + msg_res_user[0][2][3]);
			msg_user_res[3][2][i] = msg_res_user[3][3][i] / (msg_res_user[3][3][0] + msg_res_user[3][3][1] +
			                                                 msg_res_user[3][3][2] + msg_res_user[3][3][3]);
			msg_user_res[3][3][i] = msg_res_user[2][3][i] / (msg_res_user[2][3][0] + msg_res_user[2][3][1] +
			                                                 msg_res_user[2][3][2] + msg_res_user[2][3][3]);
			msg_user_res[4][0][i] = msg_res_user[3][4][i] / (msg_res_user[3][4][0] + msg_res_user[3][4][1] +
			                                                 msg_res_user[3][4][2] + msg_res_user[3][4][3]);
			msg_user_res[4][3][i] = msg_res_user[0][4][i] / (msg_res_user[0][4][0] + msg_res_user[0][4][1] +
			                                                 msg_res_user[0][4][2] + msg_res_user[0][4][3]);
			msg_user_res[5][1][i] = msg_res_user[2][5][i] / (msg_res_user[2][5][0] + msg_res_user[2][5][1] +
			                                                 msg_res_user[2][5][2] + msg_res_user[2][5][3]);
			msg_user_res[5][2][i] = msg_res_user[1][5][i] / (msg_res_user[1][5][0] + msg_res_user[1][5][1] +
			                                                 msg_res_user[1][5][2] + msg_res_user[1][5][3]);
		}
	}
	// end of iterations

	// guess at each user
	for (auto i = 0; i < 4; i++) //codeword index
	{
		guess[0][i] = msg_res_user[3][0][i] * msg_res_user[1][0][i];
		guess[1][i] = msg_res_user[2][1][i] * msg_res_user[0][1][i];
		guess[2][i] = msg_res_user[0][2][i] * msg_res_user[1][2][i];
		guess[3][i] = msg_res_user[2][3][i] * msg_res_user[3][3][i];
		guess[4][i] = msg_res_user[3][4][i] * msg_res_user[0][4][i];
		guess[5][i] = msg_res_user[1][5][i] * msg_res_user[2][5][i];
	}

	// LLRs computation
	Y_N2[0 * this->N + batch *2 +0] = (Q)(std::log(guess[0][0] + guess[0][2]) - std::log(guess[0][1] + guess[0][3]));
	Y_N2[1 * this->N + batch *2 +0] = (Q)(std::log(guess[1][0] + guess[1][2]) - std::log(guess[1][1] + guess[1][3]));
	Y_N2[2 * this->N + batch *2 +0] = (Q)(std::log(guess[2][0] + guess[2][2]) - std::log(guess[2][1] + guess[2][3]));
	Y_N2[3 * this->N + batch *2 +0] = (Q)(std::log(guess[3][0] + guess[3][2]) - std::log(guess[3][1] + guess[3][3]));
	Y_N2[4 * this->N + batch *2 +0] = (Q)(std::log(guess[4][0] + guess[4][2]) - std::log(guess[4][1] + guess[4][3]));
	Y_N2[5 * this->N + batch *2 +0] = (Q)(std::log(guess[5][0] + guess[5][2]) - std::log(guess[5][1] + guess[5][3]));

	if((this->N % 2) != 1 || batch != ((this->N /2 +1) -1))
	{
		Y_N2[0 * this->N + batch *2 +1] = (Q)(std::log(guess[0][0] + guess[0][1]) - std::log(guess[0][2] + guess[0][3]));
		Y_N2[1 * this->N + batch *2 +1] = (Q)(std::log(guess[1][0] + guess[1][1]) - std::log(guess[1][2] + guess[1][3]));
		Y_N2[2 * this->N + batch *2 +1] = (Q)(std::log(guess[2][0] + guess[2][1]) - std::log(guess[2][2] + guess[2][3]));
		Y_N2[3 * this->N + batch *2 +1] = (Q)(std::log(guess[3][0] + guess[3][1]) - std::log(guess[3][2] + guess[3][3]));
		Y_N2[4 * this->N + batch *2 +1] = (Q)(std::log(guess[4][0] + guess[4][1]) - std::log(guess[4][2] + guess[4][3]));
		Y_N2[5 * this->N + batch *2 +1] = (Q)(std::log(guess[5][0] + guess[5][1]) - std::log(guess[5][2] + guess[5][3]));
	}
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
Q Modem_SCMA<B,R,Q,PSI>
::phi(const Q* Y_N1, int i, int j, int k, int re, int batch)
{
	Q phi;
	std::complex<Q> tmp;

	auto Y_N = std::complex<Q>(Y_N1[batch *8 + 2*re], Y_N1[batch*8 + 2*re +1]);

	const auto CB0 = std::complex<Q>((Q)CB[re_user[re][0]][re][i].real(), (Q)CB[re_user[re][0]][re][i].imag());
	const auto CB1 = std::complex<Q>((Q)CB[re_user[re][1]][re][j].real(), (Q)CB[re_user[re][1]][re][j].imag());
	const auto CB2 = std::complex<Q>((Q)CB[re_user[re][2]][re][k].real(), (Q)CB[re_user[re][2]][re][k].imag());

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

	const auto H_N0 = std::complex<Q>((Q)H_N[re_user[re][0] * Nmod + 8 * batch + 2 * re   ],
	                                  (Q)H_N[re_user[re][0] * Nmod + 8 * batch + 2 * re +1]);
	const auto H_N1 = std::complex<Q>((Q)H_N[re_user[re][1] * Nmod + 8 * batch + 2 * re   ],
	                                  (Q)H_N[re_user[re][1] * Nmod + 8 * batch + 2 * re +1]);
	const auto H_N2 = std::complex<Q>((Q)H_N[re_user[re][2] * Nmod + 8 * batch + 2 * re   ],
	                                  (Q)H_N[re_user[re][2] * Nmod + 8 * batch + 2 * re +1]);

	const auto CB0  = std::complex<Q>((Q)CB[re_user[re][0]][re][i].real(), (Q)CB[re_user[re][0]][re][i].imag());
	const auto CB1  = std::complex<Q>((Q)CB[re_user[re][1]][re][j].real(), (Q)CB[re_user[re][1]][re][j].imag());
	const auto CB2  = std::complex<Q>((Q)CB[re_user[re][2]][re][k].real(), (Q)CB[re_user[re][2]][re][k].imag());

	tmp = Y_N - (H_N0 * CB0 + H_N1 * CB1 + H_N2 * CB2);

	phi = PSI(tmp, (Q)n0);

	return phi;
}

template <typename B, typename R, typename Q, tools::proto_psi<Q> PSI>
void Modem_SCMA<B,R,Q,PSI>
::filter(const R *Y_N1, R *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N_fil + this->n_frames, Y_N2);
}
}
}
