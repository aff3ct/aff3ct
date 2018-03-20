#include <typeinfo>
#include <limits>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_layered_approximate_min_star.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_min<R> MIN>
Decoder_LDPC_BP_layered_approximate_min_star<B,R,MIN>
::Decoder_LDPC_BP_layered_approximate_min_star(const int K, const int N, const int n_ite,
                                               const tools::Sparse_matrix &H,
                                               const std::vector<unsigned> &info_bits_pos,
                                               const bool enable_syndrome,
                                               const int syndrome_depth,
                                               const int n_frames)
: Decoder(K, N, n_frames, 1),
  Decoder_LDPC_BP_layered<B,R>(K, N, n_ite, H, info_bits_pos, enable_syndrome, syndrome_depth, n_frames),
  contributions(H.get_cols_max_degree()), values(H.get_cols_max_degree())
{
	const std::string name = "Decoder_LDPC_BP_layered_approximate_min_star";
	this->set_name(name);

	if (typeid(R) != typeid(float) && typeid(R) != typeid(double))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "This decoder only supports floating-point LLRs.");
}

template <typename B, typename R, tools::proto_min<R> MIN>
Decoder_LDPC_BP_layered_approximate_min_star<B,R,MIN>
::~Decoder_LDPC_BP_layered_approximate_min_star()
{
}

// BP algorithm
template <typename B, typename R, tools::proto_min<R> MIN>
void Decoder_LDPC_BP_layered_approximate_min_star<B,R,MIN>
::BP_process(std::vector<R> &var_nodes, std::vector<R> &branches)
{
	auto kr = 0;
	auto kw = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign     = 0;
		auto min      = std::numeric_limits<R>::max();
		auto deltaMin = std::numeric_limits<R>::max();

		const auto n_VN = (int)this->H[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j]  = var_nodes[this->H[i][j]] - branches[kr++];
			const auto v_abs  = (R)std::abs(contributions[j]);
			const auto c_sign = std::signbit((float)contributions[j]) ? -1 : 0;
			const auto v_temp = min;

			sign    ^= c_sign;
			min      = std::min(min, v_abs);
			deltaMin = MIN(deltaMin, (v_abs == min) ? v_temp : v_abs);
		}

		auto delta = MIN     (deltaMin, min );
		delta      = std::max((R)0, delta   );
		deltaMin   = std::max((R)0, deltaMin);

		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = contributions[j];
			const auto v_abs = (R)std::abs(value);
			      auto v_res = ((v_abs == min) ? deltaMin : delta);          // cmov
			const auto v_sig = sign ^ (std::signbit((float)value) ? -1 : 0); // xor bit
			           v_res = (R)std::copysign(v_res, v_sig);               // magnitude of v_res, sign of v_sig

			branches[kw++] = v_res;
			var_nodes[this->H[i][j]] = contributions[j] + v_res;
		}
	}
}
}
}
