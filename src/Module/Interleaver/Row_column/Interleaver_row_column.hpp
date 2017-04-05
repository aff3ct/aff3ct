#ifndef INTERLEAVER_ROW_COLUMN_HPP
#define	INTERLEAVER_ROW_COLUMN_HPP

#include <stdexcept>
#include <algorithm>
#include <time.h>
#include <random>

#include "../Interleaver.hpp"
#include "Tools/Math/matrix.h"


namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_row_column : public Interleaver<T>
{
private:
	const int n_cols;
	const int n_rows;

public:
	Interleaver_row_column(const int size, const int n_cols, const int n_frames = 1,
	                       const std::string name = "Interleaver_row_column")
	: Interleaver<T>(size, n_frames, name), n_cols(n_cols), n_rows(size / n_cols)
	{
		if (n_rows * n_cols != size)
			throw std::invalid_argument("aff3ct::module::Interleaver_row_column: \"size\" has to be equal to "
			                            "\"n_rows\" * \"n_cols\".");

		gen_lookup_tables();
	}

	void gen_lookup_tables()
	{
		for (auto i = 0; i < n_cols; i++)
			for (auto j = 0; j < n_rows; j++)
				this->pi[i*n_rows+j] = j*n_cols+i;

		for (auto i = 0; i < (int)this->pi_inv.size(); i++)
			this->pi_inv[this->pi[i]] = i;
	}
};
}
}

#endif	/* INTERLEAVER_ROW_COLUMN_HPP */
