#include <limits>

#include "Tools/Display/bash_tools.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP_flooding_sum_product.hpp"
using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_LDPC_BP_flooding_sum_product<B,R>
::Decoder_LDPC_BP_flooding_sum_product(const int &K, const int &N, const int& n_ite,
                                       const AList_reader &alist_data,
                                       const bool enable_syndrome,
                                       const int n_frames,
                                       const std::string name)
: Decoder_LDPC_BP_flooding<B,R>(K, N, n_ite, alist_data, enable_syndrome, n_frames, name),
  values(alist_data.get_CN_max_degree())
{
}

template <typename B, typename R>
Decoder_LDPC_BP_flooding_sum_product<B,R>
::~Decoder_LDPC_BP_flooding_sum_product()
{
}

// log sum-product implementation
template <typename B, typename R>
bool Decoder_LDPC_BP_flooding_sum_product<B,R>
::BP_process(const mipp::vector<R> &Y_N, mipp::vector<R> &V_to_C, mipp::vector<R> &C_to_V)
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

		auto sign =    0;
		auto prod = (R)1;

		// accumulate the incoming information in CN
		for (auto j = 0; j < length; j++)
		{
			const auto value  = V_to_C[transpose_ptr[j]];
			const auto v_abs  = (R)std::abs(value);
			const auto res    = (R)std::tanh(v_abs * (R)0.5);
			const auto c_sign = std::signbit((float)value) ? -1 : 0;

			sign ^= c_sign;
			prod  *= res;
			values[j] = res;
		}

		// regenerate the CN outcoming values
		for (auto j = 0; j < length; j++)
		{
			const auto value = V_to_C[transpose_ptr[j]];
			const auto v_sig = sign ^ (std::signbit((float)value) ? -1 : 0);
			      auto val   = prod / values[j];
			           val   = (val < (R)1.0) ? val : (R)1.0 - std::numeric_limits<R>::epsilon();
			const auto v_tan = (R)2.0 * std::atanh(val);
			const auto v_res = (R)std::copysign(v_tan, v_sig);

			C_to_V[transpose_ptr[j]] = v_res;
		}

		transpose_ptr += length;
		syndrome = syndrome || sign;
	}

	return (syndrome == 0);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_flooding_sum_product<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_sum_product<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_sum_product<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_flooding_sum_product<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_flooding_sum_product<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
