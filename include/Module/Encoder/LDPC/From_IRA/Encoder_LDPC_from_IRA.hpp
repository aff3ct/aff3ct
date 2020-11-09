/*!
 * \file
 * \brief Class module::Encoder_LDPC_from_IRA.
 */
#ifndef ENCODER_LDPC_FROM_IRA_HPP_
#define ENCODER_LDPC_FROM_IRA_HPP_

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_IRA : public Encoder_LDPC<B>
{
public:
	Encoder_LDPC_from_IRA(const int K, const int N, const tools::Sparse_matrix &H);
	virtual ~Encoder_LDPC_from_IRA() = default;

	virtual Encoder_LDPC_from_IRA<B>* clone() const;

protected:
	void _encode(const B *U_K, B *X_N, const size_t frame_id);
	void _check_H_dimensions();
};

}
}

#endif /* ENCODER_LDPC_FROM_IRA_HPP_ */
