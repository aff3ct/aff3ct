#ifndef LDPC_SYNDROME_HPP_
#define LDPC_SYNDROME_HPP_

#include <vector>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct LDPC_syndrome
{
public:
	template <typename B>
	static inline bool check_hard(const std::vector<B> &X_N, const Sparse_matrix &H);

	template <typename B>
	static inline bool check_hard(const B *X_N, const Sparse_matrix &H);

	template <typename R>
	static inline bool check_soft(const std::vector<R> &Y_N, const Sparse_matrix &H);

	template <typename R>
	static inline bool check_soft(const R *Y_N, const Sparse_matrix &H);
};
}
}

#include "LDPC_syndrome.hxx"

#endif /* LDPC_SYNDROME_HPP_ */
