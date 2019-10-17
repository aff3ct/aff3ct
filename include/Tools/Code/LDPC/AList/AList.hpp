/*!
 * \file
 * \brief Struct tools::AList.
 */
#ifndef ALIST_HPP_
#define ALIST_HPP_

#include <iostream>
#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct AList
{
public:
	static Sparse_matrix read (                             std::istream &stream);
	static void          write(const Sparse_matrix &matrix, std::ostream &stream, bool zero_padding = false);

	/*
	 * get the matrix dimensions H and N from the input stream
	 * @H is the height of the matrix
	 * @N is the width of the matrix
	 */
	static void                  read_matrix_size   (std::istream &stream, int& H, int& N);

	static std::vector<unsigned> read_info_bits_pos (std::istream &stream);
	static std::vector<unsigned> read_info_bits_pos (std::istream &stream, const int K, const int N);
	static void                  write_info_bits_pos(const std::vector<unsigned> &info_bits_pos, std::ostream &stream);

private:
	static Sparse_matrix read_format1(std::istream &stream);
	static Sparse_matrix read_format2(std::istream &stream);
	static Sparse_matrix read_format3(std::istream &stream);
};
}
}

#endif /* ALIST_HPP_ */
