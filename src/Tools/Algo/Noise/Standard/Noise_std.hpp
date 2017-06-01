#ifndef NOISE_STD_HPP_
#define NOISE_STD_HPP_

#include <random>

#include "../Noise.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Noise_std : public Noise<R>
{
private:
//	std::minstd_rand            rd_engine; // LCG
	std::mt19937                rd_engine; // Mersenne Twister 19937
	std::normal_distribution<R> normal_dist;

public:
	Noise_std(const int seed = 0);
	virtual ~Noise_std();

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma);
};
}
}

#endif /* NOISE_STD_HPP_ */
