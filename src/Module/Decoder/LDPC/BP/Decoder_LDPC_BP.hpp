#ifndef DECODER_LDPC_BP_HPP_
#define DECODER_LDPC_BP_HPP_

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Syndrome/LDPC_syndrome.hpp"

namespace aff3ct
{
namespace module
{
class Decoder_LDPC_BP
{
protected:
	const int                   n_ite;
	const tools::Sparse_matrix &H;
	const bool                  enable_syndrome;
	const int                   syndrome_depth;

	int cur_syndrome_depth;

public:
	Decoder_LDPC_BP(const int K, const int N, const int n_ite,
	                const tools::Sparse_matrix &H,
	                const bool enable_syndrome = true,
	                const int syndrome_depth = 1);

	virtual ~Decoder_LDPC_BP();

	template <typename R>
	inline bool check_syndrome_soft(const R* Y_N)
	{
		if (this->enable_syndrome)
		{
			const auto syndrome = tools::LDPC_syndrome::check_soft(Y_N, this->H);
			this->cur_syndrome_depth = syndrome ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;
			return syndrome && (this->cur_syndrome_depth == 0);
		}
		else
			return false;
	}

	template <typename B>
	inline bool check_syndrome_hard(const B* V_N)
	{
		if (this->enable_syndrome)
		{
			const auto syndrome = tools::LDPC_syndrome::check_hard(V_N, this->H);
			this->cur_syndrome_depth = syndrome ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;
			return syndrome && (this->cur_syndrome_depth == 0);
		}
		else
			return false;
	}
};
}
}

#endif /* DECODER_LDPC_BP_HPP_ */
