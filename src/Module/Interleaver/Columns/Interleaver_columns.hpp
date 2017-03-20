#ifndef INTERLEAVER_COLUMNS_HPP
#define	INTERLEAVER_COLUMNS_HPP

#include <algorithm>
#include <time.h>
#include <random>

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_columns : public Interleaver<T>
{
private:
//	std::random_device rd;
//	std::mt19937       rd_engine;

	const int nbr_columns;
	int columns_size;

public:
	Interleaver_columns(const int size, const int nbr_columns, const int seed = 0,
	                    const std::string name = "Interleaver_columns")
	: Interleaver<T>(size, 1, name),/* rd(), rd_engine(rd()),*/ nbr_columns(nbr_columns)
	{
		columns_size = (size/nbr_columns);
		assert(columns_size * nbr_columns == size);
		//rd_engine.seed(seed);
		gen_lookup_tables();
	}

	void gen_lookup_tables()
	{
		mipp::vector<T> pi_temp(columns_size); // interleaver lookup table for a column

		for (auto column = 0; column < nbr_columns; column++)
		{
			for (auto c = 0; c < columns_size; c++)
				pi_temp[c] = c + nbr_columns * columns_size;

			//std::shuffle(pi_temp.begin(), pi_temp.end(), rd_engine);

			for (auto c = 0; c < columns_size; c++)
				this->pi[c + nbr_columns * columns_size] = pi_temp[c];
		}

		for (auto i = 0; i < (int)this->pi_inv.size(); i++)
			this->pi_inv[this->pi[i]] = i;
	}
};
}
}

#endif	/* INTERLEAVER_COLUMNS_HPP */
