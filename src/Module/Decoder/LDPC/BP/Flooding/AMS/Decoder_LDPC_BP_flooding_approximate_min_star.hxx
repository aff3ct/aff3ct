#include <limits>
#include <sstream>
#include <typeinfo>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_approximate_min_star.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_min<R> MIN>
Decoder_LDPC_BP_flooding_approximate_min_star<B,R,MIN>
::Decoder_LDPC_BP_flooding_approximate_min_star(const int K, const int N, const int n_ite,
                                                const tools::Sparse_matrix &_H,
                                                const std::vector<unsigned> &info_bits_pos,
                                                const bool enable_syndrome,
                                                const int syndrome_depth,
                                                const int n_frames)
: Decoder(K, N, n_frames, 1),
  Decoder_LDPC_BP_flooding<B,R>(K, N, n_ite, _H, info_bits_pos, enable_syndrome, syndrome_depth, n_frames)
{
	const std::string name = "Decoder_LDPC_BP_flooding_approximate_min_star";
	this->set_name(name);

	if (typeid(R) != typeid(float) && typeid(R) != typeid(double))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "This decoder only supports floating-point LLRs.");
}

template <typename B, typename R, tools::proto_min<R> MIN>
Decoder_LDPC_BP_flooding_approximate_min_star<B,R,MIN>
::~Decoder_LDPC_BP_flooding_approximate_min_star()
{
}

// normalized offest min-sum implementation
template <typename B, typename R, tools::proto_min<R> MIN>
void Decoder_LDPC_BP_flooding_approximate_min_star<B,R,MIN>
::BP_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V)
{
	// beginning of the iteration upon all the matrix lines
	R *C_to_V_ptr = C_to_V.data();
	R *V_to_C_ptr = V_to_C.data();

	for (auto i = 0; i < this->n_V_nodes; i++)
	{
		// VN node accumulate all the incoming messages
		const auto length = this->n_parities_per_variable[i];

		auto sum_C_to_V = (R)0;
		for (auto j = 0; j < length; j++)
			sum_C_to_V += C_to_V_ptr[j];

		// update the intern values
		const auto temp = Y_N[i] + sum_C_to_V;

		// generate the outcoming messages to the CNs
		for (auto j = 0; j < length; j++)
			V_to_C_ptr[j] = temp - C_to_V_ptr[j];

		C_to_V_ptr += length; // jump to the next node
		V_to_C_ptr += length; // jump to the next node
	}

	auto transpose_ptr = this->transpose.data();

	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		const auto length = this->n_variables_per_parity[i];

		auto sign     = 0;
		auto min      = std::numeric_limits<R>::max();
		auto deltaMin = std::numeric_limits<R>::max();

		// accumulate the incoming information in CN
		for (auto j = 0; j < length; j++)
		{
			const auto value  = V_to_C[transpose_ptr[j]];
			const auto c_sign = std::signbit((float)value) ? -1 : 0;
			const auto v_abs  = (R)std::abs(value);
			const auto v_temp = min;

			sign    ^= c_sign;
			min      = std::min(min, v_abs);
			deltaMin = MIN(deltaMin, (v_abs == min) ? v_temp : v_abs);
		}

		auto delta = MIN(deltaMin, min);
		delta    = std::max((R)0, delta   );
		deltaMin = std::max((R)0, deltaMin);

		// regenerate the CN outcoming values
		for (auto j = 0; j < length; j++)
		{
			const auto value   = V_to_C[transpose_ptr[j]];
			const auto v_abs   = (R)std::abs(value);
			const auto v_res   = ((v_abs == min) ? deltaMin : delta);          // cmov
			const auto v_sig   = sign ^ (std::signbit((float)value) ? -1 : 0); // xor bit
			const auto v_to_st = (R)std::copysign(v_res, v_sig);               // magnitude of v_res, sign of v_sig

			C_to_V[transpose_ptr[j]] = v_to_st;
		}

		transpose_ptr += length;
	}
}
}
}
