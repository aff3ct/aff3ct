#ifndef INTERLEAVER_ROW_COLUMN_HPP
#define	INTERLEAVER_ROW_COLUMN_HPP

#include <algorithm>
#include <time.h>
#include <random>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/matrix.h"

#include "../Interleaver.hpp"

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
	: Interleaver<T>(size, false, n_frames, name), n_cols(n_cols), n_rows(size / n_cols)
	{
		if (n_rows * n_cols != size)
		{
			std::stringstream message;
			message << "'size' has to be equal to 'n_rows' * 'n_cols' ('size' = " << size
			        << ", 'n_rows' = " << n_rows << ", 'n_cols' = " << n_cols << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual ~Interleaver_row_column()
	{
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		for (auto i = 0; i < n_cols; i++)
			for (auto j = 0; j < n_rows; j++)
				lut[i * n_rows +j] = j * n_cols +i;
	}
};
}
}

#endif	/* INTERLEAVER_ROW_COLUMN_HPP */
