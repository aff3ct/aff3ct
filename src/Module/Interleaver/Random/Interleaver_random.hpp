#ifndef INTERLEAVER_RANDOM_HPP
#define	INTERLEAVER_RANDOM_HPP

#include <algorithm>
#include <numeric>
#include <random>

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_random : public Interleaver<T>
{
private:
	std::mt19937 rd_engine;

public:
	Interleaver_random(const int size, const int seed = 0, const bool uniform = false, const int n_frames = 1,
	                   const std::string name = "Interleaver_random")
	: Interleaver<T>(size, uniform, n_frames, name), rd_engine()
	{
		rd_engine.seed(seed);
	}

	virtual ~Interleaver_random()
	{
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		std::iota   (lut, lut + this->get_size(), 0);
		std::shuffle(lut, lut + this->get_size(), rd_engine);
	}
};
}
}

#endif	/* INTERLEAVER_RANDOM_HPP */

