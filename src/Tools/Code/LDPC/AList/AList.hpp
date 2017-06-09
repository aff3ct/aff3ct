#ifndef ALIST_HPP_
#define ALIST_HPP_

#include <iostream>
#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"


namespace aff3ct
{
namespace tools
{
struct AList
{
public:
	static Sparse_matrix read (                             std::istream &stream);
	static void          write(const Sparse_matrix &matrix, std::ostream &stream);

private:
	static Sparse_matrix read_format1(std::istream &stream);
	static Sparse_matrix read_format2(std::istream &stream);
	static Sparse_matrix read_format3(std::istream &stream);
};
}
}

#endif /* ALIST_HPP_ */
