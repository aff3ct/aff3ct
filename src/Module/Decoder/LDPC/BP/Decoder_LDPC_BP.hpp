#ifndef DECODER_LDPC_BP_HPP_
#define DECODER_LDPC_BP_HPP_

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "../../Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP : public Decoder_SISO_SIHO<B,R>
{
protected:
	const int                   n_ite;
	const tools::Sparse_matrix  H; // CN are along the columns -> H.get_n_cols() == M (often M=N-K)
	                               // VN are along the rows    -> H.get_n_rows() == N
	                               // automatically transpose in the constructor if needed
	const bool                  enable_syndrome;
	const int                   syndrome_depth;

	int cur_syndrome_depth;

public:
	Decoder_LDPC_BP(const int K,
	                const int N,
	                const int n_ite,
	                const tools::Sparse_matrix &H,
	                const bool enable_syndrome = true,
	                const int syndrome_depth = 1,
	                const int n_frames = 1,
	                const int simd_inter_frame_level = 1);
	virtual ~Decoder_LDPC_BP();

	template <typename T>
	bool check_syndrome_soft(const T* Y_N)
	{
		if (this->enable_syndrome)
		{
			auto syndrome = false;

			const auto n_CN = (int)this->H.get_n_cols();
			for (auto i = 0; i < n_CN; i++)
			{
				auto sign = 0;

				const auto n_VN = (int)this->H[i].size();
				for (auto j = 0; j < n_VN; j++)
				{
					const auto value = Y_N[this->H[i][j]];
					const auto tmp_sign = std::signbit((float)value) ? -1 : 0;

					sign ^= tmp_sign;
				}

				syndrome = syndrome || sign;
			}

			this->cur_syndrome_depth = (syndrome == 0) ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;

			return (syndrome == 0) && (this->cur_syndrome_depth == 0);
		}

		return false;
	}

	template <typename T>
	bool check_syndrome_hard(const T* V_N)
	{
		if (this->enable_syndrome)
		{
			auto syndrome = false;

			const auto n_CN = (int)this->H.get_n_cols();
			for (auto i = 0; i < n_CN; i++)
			{
				auto sign = 0;

				const auto n_VN = (int)this->H[i].size();
				for (auto j = 0; j < n_VN; j++)
				{
					const auto bit = V_N[this->H[i][j]];
					const auto tmp_sign = bit ? -1 : 0;

					sign ^= tmp_sign;
				}

				syndrome = syndrome || sign;
			}

			this->cur_syndrome_depth = (syndrome == 0) ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;

			return (syndrome == 0) && (this->cur_syndrome_depth == 0);
		}

		return false;
	}
};
}
}

#endif /* DECODER_LDPC_BP_HPP_ */
