#include <iostream>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Math/matrix.h"

#include "Encoder_LDPC_DVBS2.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_DVBS2<B>
::Encoder_LDPC_DVBS2(const int K, const int N, const int n_frames)
: Encoder_LDPC<B>(K, N, n_frames)
{
	const std::string name = "Encoder_LDPC_DVBS2";
	this->set_name(name);
	
	build_dvbs2();

	if (!dvbs2)
	{
		std::stringstream message;
		message << "The given format doesn't match any known generator matrix ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (K != dvbs2->K_LDPC)
	{
		std::stringstream message;
		message << "'K' has to be equal to 'dvbs2->K_LDPC' ('K' = " << K
		        << ", 'dvbs2->K_LDPC' = " << dvbs2->K_LDPC << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != dvbs2->N_LDPC)
	{
		std::stringstream message;
		message << "'N' has to be equal to 'dvbs2->N_LDPC' ('N' = " << N
		        << ", 'dvbs2->N_LDPC' = " << dvbs2->N_LDPC << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
Encoder_LDPC_DVBS2<B>
::~Encoder_LDPC_DVBS2()
{
	if (dvbs2)
		delete dvbs2;
}

template <typename B>
void Encoder_LDPC_DVBS2<B>
::build_dvbs2()
{
	dvbs2 = nullptr;

	auto NmK = this->N - this->K;

	switch (this->N)
	{
		case 16200:
		{
			switch (NmK)
			{
				case 1800 :
					dvbs2 = new dvbs2_values_16200_1800();
					break;
				case 2880 :
					dvbs2 = new dvbs2_values_16200_2880();
					break;
				case 3600 :
					dvbs2 = new dvbs2_values_16200_3600();
					break;
				case 4320 :
					dvbs2 = new dvbs2_values_16200_4320();
					break;
				case 5400 :
					dvbs2 = new dvbs2_values_16200_5400();
					break;
				case 6480 :
					dvbs2 = new dvbs2_values_16200_6480();
					break;
				case 9000 :
					dvbs2 = new dvbs2_values_16200_9000();
					break;
				case 9720 :
					dvbs2 = new dvbs2_values_16200_9720();
					break;
				case 10800 :
					dvbs2 = new dvbs2_values_16200_10800();
					break;
				case 12960 :
					dvbs2 = new dvbs2_values_16200_12960();
					break;
				default :
					break;
			}
			break;
		}
		case 64800:
		{
			switch (NmK)
			{
				case 6480 :
					dvbs2 = new dvbs2_values_64800_6480();
					break;
				case 7200 :
					dvbs2 = new dvbs2_values_64800_7200();
					break;
				case 10800 :
					dvbs2 = new dvbs2_values_64800_10800();
					break;
				case 12960 :
					dvbs2 = new dvbs2_values_64800_12960();
					break;
				case 16200 :
					dvbs2 = new dvbs2_values_64800_16200();
					break;
				case 21600 :
					dvbs2 = new dvbs2_values_64800_21600();
					break;
				case 25920 :
					dvbs2 = new dvbs2_values_64800_25920();
					break;
				case 32400 :
					dvbs2 = new dvbs2_values_64800_32400();
					break;
				case 38880 :
					dvbs2 = new dvbs2_values_64800_38880();
					break;
				case 43200 :
					dvbs2 = new dvbs2_values_64800_43200();
					break;
				case 48600 :
					dvbs2 = new dvbs2_values_64800_48600();
					break;
				default :
					break;
			}
			break;
		}
		default:
			break;
	}
}

template <typename B>
void Encoder_LDPC_DVBS2<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	std::copy(U_K,           U_K + this->K, X_N);
	std::fill(X_N + this->K, X_N + this->N, 0  );

	B* Px = &X_N[this->K];
	const int *p = dvbs2->EncValues.data();
	int xPos = 0;

	for (int y = 0; y < dvbs2->N_LINES; y++)
	{
		int nbPos = (*p++);

		for (int l = 0; l < dvbs2->M_LDPC; l++)
		{
			int bit = (int)U_K[xPos];
			if (bit == 1)
			{
				for (int q = 0; q < nbPos; q++)
				{
					int position  = (p[q] + (xPos % 360) * dvbs2->Q_LDPC) % dvbs2->NmK_LDPC;
					Px[position] ^= bit;
				}
			}
			xPos++;
		}
		p += nbPos;
	}

	for (int i = 1; i < dvbs2->NmK_LDPC; i++)
		Px[i] = Px[i] ^ Px[i-1];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_LDPC_DVBS2<B_8>;
template class aff3ct::module::Encoder_LDPC_DVBS2<B_16>;
template class aff3ct::module::Encoder_LDPC_DVBS2<B_32>;
template class aff3ct::module::Encoder_LDPC_DVBS2<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_DVBS2<B>;
#endif
// ==================================================================================== explicit template instantiation
