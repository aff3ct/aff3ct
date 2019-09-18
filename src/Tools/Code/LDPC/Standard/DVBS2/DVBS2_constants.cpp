#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants.hpp"
#include "Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants_64800.hpp"
#include "Tools/Code/LDPC/Standard/DVBS2/DVBS2_constants_16200.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Sparse_matrix aff3ct::tools::build_H(const dvbs2_values& dvbs2)
{
	Sparse_matrix H(dvbs2.N, dvbs2.NmK);

	const int *p = dvbs2.EncValues.data();
	int xPos = 0, yPos;

	for (int y = 0; y < dvbs2.N_LINES; y++)
	{
		int nbPos = (*p++);

		for (int l = 0; l < dvbs2.M; l++)
		{

			for (int q = 0; q < nbPos; q++)
			{
				// yPos  = (p[q] + (xPos % dvbs2.M) * dvbs2.Q) % dvbs2.NmK;
				yPos  = (p[q] + l * dvbs2.Q) % dvbs2.NmK;

				H.add_connection(xPos, yPos);
			}
			xPos++;
		}
		p += nbPos;
	}


	for (xPos = dvbs2.K; xPos < dvbs2.N; xPos++)
	{
		yPos = xPos - dvbs2.K;

		H.add_connection(xPos, yPos);
		yPos++;

		if (yPos < dvbs2.NmK)
				H.add_connection(xPos, yPos);
	}

	return H;
}

std::unique_ptr<dvbs2_values> aff3ct::tools::build_dvbs2(const int K, const int N)
{
	std::unique_ptr<dvbs2_values> dvbs2;

	auto NmK = N - K;

	switch (N)
	{
		case 16200:
		{
			switch (NmK)
			{
				case 1800 : dvbs2.reset(new tools::dvbs2_values_16200_1800 ()); break;
				case 2880 : dvbs2.reset(new tools::dvbs2_values_16200_2880 ()); break;
				case 3600 : dvbs2.reset(new tools::dvbs2_values_16200_3600 ()); break;
				case 4320 : dvbs2.reset(new tools::dvbs2_values_16200_4320 ()); break;
				case 5400 : dvbs2.reset(new tools::dvbs2_values_16200_5400 ()); break;
				case 6480 : dvbs2.reset(new tools::dvbs2_values_16200_6480 ()); break;
				case 9000 : dvbs2.reset(new tools::dvbs2_values_16200_9000 ()); break;
				case 9720 : dvbs2.reset(new tools::dvbs2_values_16200_9720 ()); break;
				case 10800: dvbs2.reset(new tools::dvbs2_values_16200_10800()); break;
				case 12960: dvbs2.reset(new tools::dvbs2_values_16200_12960()); break;
				default   : break;
			}
			break;
		}
		case 64800:
		{
			switch (NmK)
			{
				case 6480 : dvbs2.reset(new tools::dvbs2_values_64800_6480 ()); break;
				case 7200 : dvbs2.reset(new tools::dvbs2_values_64800_7200 ()); break;
				case 10800: dvbs2.reset(new tools::dvbs2_values_64800_10800()); break;
				case 12960: dvbs2.reset(new tools::dvbs2_values_64800_12960()); break;
				case 16200: dvbs2.reset(new tools::dvbs2_values_64800_16200()); break;
				case 21600: dvbs2.reset(new tools::dvbs2_values_64800_21600()); break;
				case 25920: dvbs2.reset(new tools::dvbs2_values_64800_25920()); break;
				case 32400: dvbs2.reset(new tools::dvbs2_values_64800_32400()); break;
				case 38880: dvbs2.reset(new tools::dvbs2_values_64800_38880()); break;
				case 43200: dvbs2.reset(new tools::dvbs2_values_64800_43200()); break;
				case 48600: dvbs2.reset(new tools::dvbs2_values_64800_48600()); break;
				default   : break;
			}
			break;
		}
		default:
			break;
	}

	if (dvbs2 == nullptr)
	{
		std::stringstream message;
		message << "The given format doesn't match any known generator matrix ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K != dvbs2->K)
	{
		std::stringstream message;
		message << "'K' has to be equal to 'dvbs2->K' ('K' = " << K
		        << ", 'dvbs2->K' = " << dvbs2->K << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != dvbs2->N)
	{
		std::stringstream message;
		message << "'N' has to be equal to 'dvbs2->N' ('N' = " << N
		        << ", 'dvbs2->N' = " << dvbs2->N << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (dvbs2->Q != (dvbs2->N - dvbs2->K)/dvbs2->M)
	{
		std::stringstream message;
		message << "'Q' has to be equal to ('N' - 'K')/'M'  ('Q' = " << dvbs2->Q << ", 'N' = " << dvbs2->N
		        << ", 'K' = " << dvbs2->K << ", 'M' = " << dvbs2->M << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (dvbs2->K != (dvbs2->M * dvbs2->N_LINES))
	{
		std::stringstream message;
		message << "'K' has to be equal to ('M' * 'N_LINES')  ('K' = " << dvbs2->K << ", 'M' = " << dvbs2->M
		        << ", 'N_LINES' = " << dvbs2->N_LINES << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return dvbs2;
}