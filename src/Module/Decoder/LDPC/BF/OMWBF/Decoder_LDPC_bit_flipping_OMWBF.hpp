#ifndef DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_
#define DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_

#include "../Decoder_LDPC_bit_flipping.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_bit_flipping_OMWBF : public Decoder_LDPC_bit_flipping<B,R>
{
//private:
//	const float normalize_factor;
//	const R offset;

public:
	Decoder_LDPC_bit_flipping_OMWBF(const int &K, const int &N, const int& n_ite,
	                              const tools::Sparse_matrix &H,
	                              const std::vector<unsigned> &info_bits_pos,
//	                              const float normalize_factor = 1.f,
	                              const R mwbf_factor = (R)0,
	                              const bool enable_syndrome = true,
	                              const int syndrome_depth = 1,
	                              const int n_frames = 1);
	virtual ~Decoder_LDPC_bit_flipping_OMWBF() = default;

protected:
	// BF functions for decoding
	virtual bool BF_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V);


	std::vector<B> synd  ;
	std::vector<R> energy;
};
}
}

#endif /* DECODER_LDPC_BIT_FLIPPING_OMWBF_HPP_ */
