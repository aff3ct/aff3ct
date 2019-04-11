#ifndef DECODER_POLAR_MK_SCL_NAIVE_CA_SYS_
#define DECODER_POLAR_MK_SCL_NAIVE_CA_SYS_

#include "Decoder_polar_MK_SCL_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_polar_MK_SCL_naive_CA_sys : public Decoder_polar_MK_SCL_naive_CA<B,R>
{

public:
	Decoder_polar_MK_SCL_naive_CA_sys(const int& K,
	                                  const int& N,
	                                  const int& L,
	                                  const tools::Polar_code& code,
	                                  const std::vector<bool>& frozen_bits,
	                                  CRC<B>& crc,
	                                  const int n_frames = 1);

	virtual ~Decoder_polar_MK_SCL_naive_CA_sys() = default;

protected:
	virtual void select_best_path();
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#endif /* DECODER_POLAR_MK_SCL_NAIVE_CA_SYS_ */
