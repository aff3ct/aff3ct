#ifndef DECODER_LDPC_BP_HPP_
#define DECODER_LDPC_BP_HPP_

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace module
{
class Decoder_LDPC_BP
{
protected:
	const int                   n_ite;
	const tools::Sparse_matrix  H; // In vertical way
	                               // CN are along the columns -> H.get_n_cols() == M (often M=N-K)
	                               // VN are along the rows    -> H.get_n_rows() == N
	                               // automatically transpose in the constructor if needed
	const bool                  enable_syndrome;
	const int                   syndrome_depth;

	int cur_syndrome_depth;

public:
	Decoder_LDPC_BP(const int K, const int N, const int n_ite,
	                const tools::Sparse_matrix &H,
	                const bool enable_syndrome = true,
	                const int syndrome_depth = 1);

	virtual ~Decoder_LDPC_BP() = default;

	template <typename R>
	inline bool check_syndrome_soft(const R* Y_N);

	template <typename B>
	inline bool check_syndrome_hard(const B* V_N);
};
}
}

#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hxx"

#endif /* DECODER_LDPC_BP_HPP_ */
