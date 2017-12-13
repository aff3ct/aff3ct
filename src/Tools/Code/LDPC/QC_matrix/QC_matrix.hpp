#ifndef QC_MATRIX_HPP_
#define QC_MATRIX_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <mipp.h>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct QC_matrix
{
private:
	std::vector<mipp::vector<int16_t>> H_red;
	std::vector<bool>                  pct_pattern;
	unsigned                           Z;

public:
	using QCFull_matrix = std::vector<mipp::vector<int8_t>>;

	QC_matrix(const unsigned N_red = 0, const unsigned M_red = 1);
	virtual ~QC_matrix();

	Sparse_matrix     expand_QC();
	std::vector<bool> get_pct_pattern() const;

	static QC_matrix  load(std::istream &stream);

	/*
	 * inverse H2 (H = [H1 H2] with size(H2) = M x M) to allow encoding with p = H1 x inv(H2) x u
	 */
	static QCFull_matrix invert_H2(const Sparse_matrix& H);
};
}
}

#endif /* QC_MATRIX_HPP_ */
