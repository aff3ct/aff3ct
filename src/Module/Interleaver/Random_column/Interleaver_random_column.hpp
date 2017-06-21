#ifndef INTERLEAVER_RANDOM_COLUMN_HPP
#define	INTERLEAVER_RANDOM_COLUMN_HPP

#include <algorithm>
#include <time.h>
#include <random>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_random_column : public Interleaver<T>
{
private:
	std::mt19937 rd_engine;

	const int n_cols;
	const int col_size;

public:
	Interleaver_random_column(const int size, const int n_cols, const int seed = 0, const bool uniform = false,
	                          const int n_frames = 1, const std::string name = "Interleaver_random_column")
	: Interleaver<T>(size, uniform, n_frames, name),
	  rd_engine(), n_cols(n_cols), col_size(size / n_cols)
	{
		if (col_size * n_cols != size)
		{
			std::stringstream message;
			message << "'size' has to be equal to 'n_cols' * 'col_size' ('size' = " << size
			        << ", 'n_cols' = " << n_cols << ", 'col_size' = " << col_size << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		rd_engine.seed(seed);
	}

	virtual ~Interleaver_random_column()
	{
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		mipp::vector<T> pi_temp(col_size); // interleaver lookup table for a column

		for (auto column = 0; column < n_cols; column++)
		{
			for (auto c = 0; c < col_size; c++)
				pi_temp[c] = c + column * col_size;

			std::shuffle(pi_temp.begin(), pi_temp.end(), rd_engine);

			for (auto c = 0; c < col_size; c++)
				lut[c + column * col_size] = pi_temp[c];
		}
	}
};
}
}

#endif	/* INTERLEAVER_RANDOM_COLUMNS_HPP */
