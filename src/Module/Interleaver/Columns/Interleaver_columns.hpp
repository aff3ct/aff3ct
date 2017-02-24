#ifndef INTERLEAVER_COLUMNS_HPP
#define	INTERLEAVER_COLUMNS_HPP

#include <algorithm>
#include <time.h>
#include <random>

#include "../Interleaver.hpp"

namespace aff3ct
{
template <typename T>
class Interleaver_columns : public Interleaver<T>
{
private:
	const int& K;
	const int& M;

public:
	Interleaver_columns(const int& K, const int& M, const std::string name = "Interleaver_columns") 
	: Interleaver<T>(K, 1, name), K(K), M(M) 
	{ 
		assert(K % M == 0);
		gen_lookup_tables(); 
	}

protected:
	void gen_lookup_tables()
	{
		std::random_device rd;
		std::mt19937 g(rd());

		auto column_number = K / M;

		mipp::vector<T> pi_temp(M);  // interleaver lookup table

		for (auto column = 0; column < column_number; column++)
		{
			for (auto row = 0; row < M; row++)
				pi_temp[row] = column + column_number * row;

			std::shuffle(pi_temp.begin(), pi_temp.end(), g);

			for (auto row = 0; row < M; row++)
				this->pi[column + column_number * row] = pi_temp[row];
		}

		for (auto i = 0; i < (int)this->pi_inv.size(); i++)
			this->pi_inv[this->pi[i]] = i;
	}
};
}

#endif	/* INTERLEAVER_COLUMNS_HPP */
