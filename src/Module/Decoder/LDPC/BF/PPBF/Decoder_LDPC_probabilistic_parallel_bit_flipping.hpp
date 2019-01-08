#ifndef DECODER_LDPC_PROBABILISTIC_PARALLEL_BIT_FLIPPING_HPP_
#define DECODER_LDPC_PROBABILISTIC_PARALLEL_BIT_FLIPPING_HPP_

#include <random>
#include <memory>

#include "../Decoder_LDPC_bit_flipping_hard.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_probabilistic_parallel_bit_flipping : public Decoder_LDPC_bit_flipping_hard<B,R>
{
public:
	Decoder_LDPC_probabilistic_parallel_bit_flipping(const int &K, const int &N, const int& n_ite,
	                                                 const tools::Sparse_matrix &H,
	                                                 const std::vector<unsigned> &info_bits_pos,
	                                                 const std::vector<float> &bernouilli_probas,
	                                                 const bool enable_syndrome = true,
	                                                 const int syndrome_depth = 1,
	                                                 const int seed = 0,
	                                                 const int n_frames = 1);
	virtual ~Decoder_LDPC_probabilistic_parallel_bit_flipping() = default;

protected:
	std::mt19937 rd_engine; // Mersenne Twister 19937
	std::vector<std::unique_ptr<std::bernoulli_distribution>> bernouilli_dist;

	virtual void cn_process(const B *VN,  B *CN,              const int frame_id);
	virtual void vn_process(const B *Y_N, B *VN, const B *CN, const int frame_id);
};

template <typename B = int, typename R = float>
using Decoder_LDPC_PPBF = Decoder_LDPC_probabilistic_parallel_bit_flipping<B,R>;

}
}

#endif /* DECODER_LDPC_PROBABILISTIC_PARALLEL_BIT_FLIPPING_HPP_ */
