#ifndef INTERLEAVER_RANDOM_COLUMN_HPP
#define	INTERLEAVER_RANDOM_COLUMN_HPP

#include <algorithm>
#include <time.h>
#include <random>

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_random_column : public Interleaver<T>
{
private:
	std::random_device rd;
	std::mt19937       rd_engine;

	const int n_cols;
	int col_size;

public:
	Interleaver_random_column(const int size, const int n_cols, const int seed = 0,
	                    const std::string name = "Interleaver_random_column")
	: Interleaver<T>(size, 1, name), rd(), rd_engine(rd()), n_cols(n_cols)
	{
		col_size = (size / n_cols);
		assert(col_size * n_cols == size);
		rd_engine.seed(seed);
		gen_lookup_tables();
	}

	void gen_lookup_tables()
	{
		mipp::vector<T> pi_temp(col_size); // interleaver lookup table for a column

		for (auto column = 0; column < n_cols; column++)
		{
			for (auto c = 0; c < col_size; c++)
				pi_temp[c] = c + column * col_size;

			std::shuffle(pi_temp.begin(), pi_temp.end(), rd_engine);

			for (auto c = 0; c < col_size; c++)
				this->pi[c + column * col_size] = pi_temp[c];
		}

		for (auto i = 0; i < (int)this->pi_inv.size(); i++)
			this->pi_inv[this->pi[i]] = i;
	}
};
}
}

#endif	/* INTERLEAVER_RANDOM_COLUMNS_HPP */
