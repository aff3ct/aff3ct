#include <chrono>
#include <limits>
#include <cmath>
#include <stdexcept>

#include "Tools/Perf/common.h"
#include "Tools/Math/utils.h"

#include "Decoder_LDPC_BP.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP<B,R>
::Decoder_LDPC_BP(const int K,
                  const int N,
                  const int n_ite,
                  const tools::Sparse_matrix &_H,
                  const bool enable_syndrome,
                  const int syndrome_depth,
                  const int n_frames,
                  const int simd_inter_frame_level)
: Decoder               (K, N, n_frames, simd_inter_frame_level),
  Decoder_SISO_SIHO<B,R>(K, N, n_frames, simd_inter_frame_level),
  n_ite                 (n_ite                                 ),
  H                     ((_H.get_n_cols() > _H.get_n_rows()) ? _H.transpose() : _H),
  enable_syndrome       (enable_syndrome                       ),
  syndrome_depth        (syndrome_depth                        ),
  cur_syndrome_depth    (0                                     )
{
	const std::string name = "Decoder_LDPC_BP";
	this->set_name(name);

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (syndrome_depth <= 0)
	{
		std::stringstream message;
		message << "'syndrome_depth' has to be greater than 0 ('syndrome_depth' = " << syndrome_depth << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N != (int)this->H.get_n_rows())
	{
		std::stringstream message;
		message << "'N' is not compatible with the H matrix ('N' = " << N << ", 'H.get_n_rows()' = "
		        << this->H.get_n_rows() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_LDPC_BP<B,R>
::~Decoder_LDPC_BP()
{
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
