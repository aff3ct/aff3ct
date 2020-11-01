/*!
 * \file
 * \brief Class module::Decoder_LDPC_BP_flooding_Gallager_B.
 */
#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_B_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_B_HPP_

#include <cstdint>
#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding_Gallager_B : public Decoder_LDPC_BP_flooding_Gallager_A<B,R>
{
public:
	Decoder_LDPC_BP_flooding_Gallager_B(const int K, const int N, const int n_ite, const tools::Sparse_matrix &H,
	                                    const std::vector<unsigned> &info_bits_pos,
	                                    const bool enable_syndrome = true,
	                                    const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_BP_flooding_Gallager_B() = default;
	virtual Decoder_LDPC_BP_flooding_Gallager_B<B,R>* clone() const;

protected:
	void _initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var,
	                            std::vector<int8_t> &var_to_chk, const int ite);
};

template <typename B = int, typename R = float>
using Decoder_LDPC_BP_flooding_GALB = Decoder_LDPC_BP_flooding_Gallager_B<B,R>;
}
}

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_B_HPP_ */
