/*!
 * \file
 * \brief Struct tools::dvbs2_values.
 *
 * Link to the DVBS2 standard:
 * https://www.etsi.org/deliver/etsi_en/302300_302399/30230701/01.04.01_60/en_30230701v010401p.pdf
 */
#ifndef DVBS2_CONSTANTS_HPP_
#define DVBS2_CONSTANTS_HPP_

#include <vector>
#include <memory>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{

struct dvbs2_values
{
	std::vector<int> EncValues;
	int N;
	int K;
	int NmK;
	int M;
	int Q;
	int N_LINES;
};

std::unique_ptr<dvbs2_values> build_dvbs2(const int K, const int N);

tools::Sparse_matrix build_H(const dvbs2_values& dvbs2);

}
}

#endif // DVBS2_CONSTANTS_HPP_
