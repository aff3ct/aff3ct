#include "Tools/Code/LDPC/Syndrome/LDPC_syndrome.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B>
bool LDPC_syndrome
::check_hard(const B *X_N, const Sparse_matrix &H)
{
	auto syndrome = false;

	const auto n_chk_nodes = (int)H.get_n_cols();
	auto c = 0;
	while (c < n_chk_nodes && !syndrome)
	{
		auto sign = 0;

		const auto chk_degree = (int)H[c].size();
		for (auto v = 0; v < chk_degree; v++)
		{
			const auto bit = X_N[H[c][v]];
			const auto tmp_sign = bit ? -1 : 0;

			sign ^= tmp_sign;
		}

		syndrome = syndrome || sign;
		c++;
	}

	return !syndrome;
}

template <typename B>
bool LDPC_syndrome
::check_hard(const std::vector<B> &X_N, const Sparse_matrix &H)
{
	return LDPC_syndrome::check_hard<B>(X_N.data(), H);
}

template <typename R>
bool LDPC_syndrome
::check_soft(const R *Y_N, const Sparse_matrix &H)
{
	auto syndrome = false;

	const auto n_chk_nodes = (int)H.get_n_cols();
	auto c = 0;
	while (c < n_chk_nodes && !syndrome)
	{
		auto sign = 0;

		const auto chk_degree = (int)H[c].size();
		for (auto v = 0; v < chk_degree; v++)
		{
			const auto llr = Y_N[H[c][v]];
			const auto tmp_sign = (llr < 0) ? -1 : 0;

			sign ^= tmp_sign;
		}

		syndrome = syndrome || sign;
		c++;
	}

	return !syndrome;
}

template <typename R>
bool LDPC_syndrome
::check_soft(const std::vector<R> &Y_N, const Sparse_matrix &H)
{
	return LDPC_syndrome::check_soft<R>(Y_N.data(), H);
}

}
}
