#ifndef ENCODER_NO_HPP_
#define ENCODER_NO_HPP_

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Encoder_NO : public Encoder<B>
{
public:
	Encoder_NO(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_NO");
	virtual ~Encoder_NO();

	void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};
}
}

#endif /* ENCODER_NO_HPP_ */
