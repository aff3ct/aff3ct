#ifndef DECODER_POLAR_MK_SCL_NAIVE_CA_
#define DECODER_POLAR_MK_SCL_NAIVE_CA_

#include "Module/CRC/CRC.hpp"

#include "../Decoder_polar_MK_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_polar_MK_SCL_naive_CA : public Decoder_polar_MK_SCL_naive<B,R>
{
protected:
	CRC<B>& crc;

public:
	Decoder_polar_MK_SCL_naive_CA(const int& K,
	                              const int& N,
	                              const int& L,
	                              const tools::Polar_code& code,
	                              const std::vector<bool>& frozen_bits,
	                              const std::vector<std::vector<std::function<R(const std::vector<R> &LLRs,
                                                                                const std::vector<B> &bits)>>> &lambdas,
	                              CRC<B>& crc,
	                              const int n_frames = 1);

	Decoder_polar_MK_SCL_naive_CA(const int& K,
	                              const int& N,
	                              const int& L,
	                              const tools::Polar_code& code,
	                              const std::vector<bool>& frozen_bits,
	                              CRC<B>& crc,
	                              const int n_frames = 1);

	virtual ~Decoder_polar_MK_SCL_naive_CA() = default;

protected:
	virtual void select_best_path();
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_CA_ */
