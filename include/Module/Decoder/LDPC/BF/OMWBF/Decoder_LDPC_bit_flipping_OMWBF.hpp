/*!
 * \file
 * \brief Class module::Decoder_LDPC_bit_flipping_OMWBF.
 */
#ifndef DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_
#define DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_

#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/LDPC/BF/Decoder_LDPC_bit_flipping.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_bit_flipping_OMWBF : public Decoder_LDPC_bit_flipping<B,R>
{
protected:
	std::vector<B> synd;
	std::vector<R> energy;

public:
	Decoder_LDPC_bit_flipping_OMWBF(const int &K, const int &N, const int& n_ite,
	                                const tools::Sparse_matrix &H,
	                                const std::vector<unsigned> &info_bits_pos,
	                                const R mwbf_factor = (R)0,
	                                const bool enable_syndrome = true,
	                                const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_bit_flipping_OMWBF() = default;
	virtual Decoder_LDPC_bit_flipping_OMWBF<B,R>* clone() const;

protected:
	// BF functions for decoding
	virtual bool BF_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V);
};
}
}

#endif /* DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_ */
