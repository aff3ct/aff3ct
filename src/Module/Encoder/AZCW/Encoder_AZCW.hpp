#ifndef ENCODER_AZCW_HPP_
#define ENCODER_AZCW_HPP_

#include <random>

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_AZCW : public Encoder<B>
{
public:
	Encoder_AZCW(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_AZCW");
	virtual ~Encoder_AZCW();

	void encode(const B *U_K, B *X_N); using Encoder<B>::encode;
};
}
}

#endif /* ENCODER_AZCW_HPP_ */
