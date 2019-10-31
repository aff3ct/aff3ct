/*!
 * \file
 * \brief Struct tools::QC.
 */
#ifndef QC_HPP_
#define QC_HPP_

#include <iostream>
#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct QC
{
public:
	static Sparse_matrix     read            (                             std::istream &stream                );
	static std::vector<bool> read_pct_pattern(                             std::istream &stream, int N_red = -1);
	static void              write           (const Sparse_matrix &matrix, std::ostream &stream                );

	/*
	 * get the matrix dimensions H and N from the input stream
	 * @H is the height of the matrix
	 * @N is the width of the matrix
	 */
	static void read_matrix_size(std::istream &stream, int& H, int& N);

private:
	static Sparse_matrix _read(std::istream &stream);
};
}
}

#endif /* QC_HPP_ */
