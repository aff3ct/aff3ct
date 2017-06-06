#ifndef INTERLEAVER_GOLDEN_HPP
#define INTERLEAVER_GOLDEN_HPP

#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>

#include "../Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_golden : public Interleaver<T>
{
private:
	std::mt19937                           gen;
	std::uniform_real_distribution<double> dist;

public:
	Interleaver_golden(int size, const int seed = 0, const bool uniform = false, const int n_frames = 1,
	                   const std::string name = "Interleaver_golden")
	: Interleaver<T>(size, uniform, n_frames, name), gen(), dist(0.0, size * 0.1)
	{
		gen.seed(seed);
	}

	virtual ~Interleaver_golden()
	{
	}

protected:
	void gen_lut(T *lut, const int frame_id)
	{
		double g  = (std::sqrt(5)-1)/2.0;
		double g2 = g * g;
		double c  = this->get_size() * g2;

		std::vector<double> tab(this->get_size());
		for (auto i = 0; i < this->get_size(); i++)
			tab[i] = std::fmod(i * c + dist(gen), this->get_size());

		std::iota(lut, lut + this->get_size(), 0);

		std::sort(lut, lut + this->get_size(), [&tab](size_t i1, size_t i2)
		{
			return tab[i1] < tab[i2];
		});
	}
};
}
}

#endif  /* INTERLEAVER_GOLDEN_HPP */
