/*!
 * \file
 * \brief Class module::Encoder_LDPC_from_QC.
 */
#ifndef ENCODER_LDPC_FROM_QC_HPP_
#define ENCODER_LDPC_FROM_QC_HPP_

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_QC : public Encoder_LDPC<B>
{
protected:
	tools::LDPC_matrix_handler::LDPC_matrix invH2;

public:
	Encoder_LDPC_from_QC(const int K, const int N, const tools::Sparse_matrix &H);
	virtual ~Encoder_LDPC_from_QC() = default;

	virtual Encoder_LDPC_from_QC<B>* clone() const;

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
	void _check_H_dimensions();
};

}
}

#endif /* ENCODER_LDPC_FROM_QC_HPP_ */
