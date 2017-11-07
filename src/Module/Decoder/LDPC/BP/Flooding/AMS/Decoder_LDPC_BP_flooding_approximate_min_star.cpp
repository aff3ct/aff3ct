#include <limits>
#include <sstream>
#include <typeinfo>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_approximate_min_star.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
inline R Correction_linear2(const R x)
{
	if (x > 2.625)
		return (R)0;
	else if (x < 1.0)
		return -0.3750 * x + 0.6825;
	else
		return -0.1875 * x + 0.5;
}

template <typename R>
inline R MinStar(const R a, const R b)
{
	return std::min(a, b) + (R)std::log1p(std::exp(-(a + b))) - (R)std::log1p(std::exp(-std::abs(a - b)));
}

template <typename R>
inline R MinStar_linear2(const R a, const R b)
{
	return (R)(std::min(a, b) + Correction_linear2(a + b) - Correction_linear2(std::abs(a - b)));
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_approximate_min_star<B,R>
::Decoder_LDPC_BP_flooding_approximate_min_star(const int &K, const int &N, const int& n_ite,
                                                    const tools::Sparse_matrix &H,
                                                    const std::vector<unsigned> &info_bits_pos,
                                                    const bool enable_syndrome,
                                                    const int syndrome_depth,
                                                    const int n_frames,
                                                    const std::string name)
: Decoder(K, N, n_frames, 1, name),
  Decoder_LDPC_BP_flooding<B,R>(K, N, n_ite, H, info_bits_pos, enable_syndrome, syndrome_depth, n_frames, name)
{
	/*if (typeid(R) == typeid(signed char))
	{
		std::stringstream message;
		message << "This decoder does not work in 8-bit fixed-point (try in 16-bit).";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}*/
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_approximate_min_star<B,R>
::~Decoder_LDPC_BP_flooding_approximate_min_star()
{
}

// normalized offest min-sum implementation
template <typename B, typename R>
bool Decoder_LDPC_BP_flooding_approximate_min_star<B,R>
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

	auto syndrome = 0;	
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
			deltaMin = MinStar_linear2(deltaMin, (v_abs == min) ? v_temp : v_abs);
		}

		auto delta = MinStar_linear2(deltaMin, min);
		delta = (delta < 0) ? 0 : delta;
		deltaMin = (deltaMin < 0) ? 0 : deltaMin;

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
		syndrome = syndrome || sign;
	}

	return (syndrome == 0);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_approximate_min_star<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_approximate_min_star<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_approximate_min_star<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_approximate_min_star<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_approximate_min_star<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
