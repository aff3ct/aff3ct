#ifndef QC_HPP_
#define QC_HPP_

#include <algorithm>
#include <iostream>
#include <vector>
#include <mipp.h>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

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
};
}
}

#endif /* QC_HPP_ */
