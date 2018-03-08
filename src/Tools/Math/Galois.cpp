#include <cmath>
#include <iostream>
#include <sstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"

#include "Galois.hpp"

using namespace aff3ct::tools;

Galois
::Galois(const int& K, const int& N)
 : K(K), N(N), m((int)std::ceil(std::log2(N))), alpha_to(N +1), index_of(N +1), p(m +1, 0)
{
	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (m != (int)std::ceil(std::log2(N +1)))
	{
		std::stringstream message;
		message << "'m' has to be equal to ceil(log2('N' +1)) ('m' = " << m << ", 'N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (m <= 1)
	{
		std::stringstream message;
		message << "'m' has to be strictly greater than 1 ('m' = " << m << ", 'N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != ((1 << m) -1))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 minus 1 ('N' = " << N << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	Select_Polynomial();
	Generate_GF();
}

Galois
::~Galois()
{
}

int Galois
::get_K() const
{
	return K;
}

int Galois
::get_N() const
{
	return N;
}

int Galois
::get_m() const
{
	return m;
}

const std::vector<int>& Galois
::get_alpha_to() const
{
	return alpha_to;
}

const std::vector<int>& Galois
::get_index_of() const
{
	return index_of;
}

const std::vector<int>& Galois
::get_p() const
{
	return p;
}

void Galois
::Select_Polynomial()
{
	p[0] = p[m] = 1;
	if      (m ==  2) p[1] = 1;
	else if (m ==  3) p[1] = 1;
	else if (m ==  4) p[1] = 1;
	else if (m ==  5) p[2] = 1;
	else if (m ==  6) p[1] = 1;
	else if (m ==  7) p[1] = 1;
	else if (m ==  8) p[4] = p[5] = p[6] = 1;
	else if (m ==  9) p[4] = 1;
	else if (m == 10) p[3] = 1;
	else if (m == 11) p[2] = 1;
	else if (m == 12) p[3] = p[4] = p[7] = 1;
	else if (m == 13) p[1] = p[3] = p[4] = 1;
	else if (m == 14) p[1] = p[11] = p[12] = 1;
	else if (m == 15) p[1] = 1;
	else if (m == 16) p[2] = p[3] = p[5] = 1;
	else if (m == 17) p[3] = 1;
	else if (m == 18) p[7] = 1;
	else if (m == 19) p[1] = p[5] = p[6] = 1;
	else if (m == 20) p[3] = 1;
}

void Galois
::Generate_GF()
{
	int i, mask;

	mask = 1;
	alpha_to[m] = 0;
	for (i = 0; i < m; i++)
	{
		alpha_to[i] = mask;
		index_of[alpha_to[i]] = i;
		if (p[i] != 0)
			alpha_to[m] ^= mask;
		mask <<= 1;
	}
	index_of[alpha_to[m]] = m;
	mask >>= 1;
	for (i = m + 1; i < N; i++)
	{
		if (alpha_to[i - 1] >= mask)
			alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
		else
			alpha_to[i] = alpha_to[i - 1] << 1;
		const auto idx = alpha_to[i];
		index_of[idx] = i;
	}
	index_of[0] = -1;
}
