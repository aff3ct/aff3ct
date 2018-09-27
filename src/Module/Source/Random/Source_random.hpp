#ifndef SOURCE_RANDOM_HPP_
#define SOURCE_RANDOM_HPP_

#include <random>

#include "../Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Source_random : public Source<B>
{
private:
	std::mt19937 rd_engine; // Mersenne Twister 19937
	// std::minstd_rand rd_engine; // LCG
#ifdef _MSC_VER
	std::uniform_int_distribution<short> uniform_dist;
#else
	std::uniform_int_distribution<B> uniform_dist;
#endif

public:
	Source_random(const int K, const int seed = 0, const int n_frames = 1);

	virtual ~Source_random() = default;

protected:
	void _generate(B *U_K, const int frame_id);
};
}
}

#endif /* SOURCE_RANDOM_HPP_ */
