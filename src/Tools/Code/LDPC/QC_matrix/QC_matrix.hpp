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
public:
	using QCFull_matrix   = std::vector<mipp::vector<int8_t>>;

	QC_matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);
	virtual ~QC_matrix();

	Sparse_matrix      expand_QC();
	std::vector<bool>  get_pct_pattern() const;

	static QC_matrix   load(std::istream &stream);

	/*
	 * inverse H2 (H = [H1 H2] with size(H2) = M x M) to allow encoding with p = H1 x inv(H2) x u
	 */
	static QCFull_matrix invert_H2(const Sparse_matrix& H);

private:
	std::vector<mipp::vector<int16_t>> 	Hred;
	std::vector<bool>					pctPattern;
	unsigned 							Z;

	static std::vector<std::string> split(const std::string &s);
	static void 					getline(std::istream &file, std::string &line);

};
}
}

#endif /* QC_MATRIX_HPP_ */
