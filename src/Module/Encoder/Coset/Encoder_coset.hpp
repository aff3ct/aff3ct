#ifndef ENCODER_COSET_HPP_
#define ENCODER_COSET_HPP_

#include <random>

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_coset : public Encoder<B>
{
private:
	std::mt19937 rd_engine; // Mersenne Twister 19937
#ifdef _MSC_VER
	std::uniform_int_distribution<short> uniform_dist;
#else
	std::uniform_int_distribution<B> uniform_dist;
#endif

public:
	Encoder_coset(const int K, const int N, const int seed = 0, const int n_frames = 1,
	              const std::string name = "Encoder_coset");
	virtual ~Encoder_coset();

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif /* ENCODER_COSET_HPP_ */
