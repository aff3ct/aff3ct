#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Decoder_LDPC_BP
::Decoder_LDPC_BP(const int K, const int N, const int n_ite,
                  const tools::Sparse_matrix &_H,
                  const bool enable_syndrome,
                  const int syndrome_depth)
: n_ite             (n_ite          ),
  H                 (_H             ),
  enable_syndrome   (enable_syndrome),
  syndrome_depth    (syndrome_depth ),
  cur_syndrome_depth(0              )
{
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