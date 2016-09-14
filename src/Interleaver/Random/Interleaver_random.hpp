#ifndef INTERLEAVER_RANDOM_HPP
#define	INTERLEAVER_RANDOM_HPP

#include <algorithm>
#include <time.h>
#include <random>

#include "../Interleaver.hpp"

template <typename T>
class Interleaver_random : public Interleaver<T>
{
private:
	const int seed;

public:
	Interleaver_random(int size, const int seed = -1, const std::string name = "Interleaver_random") 
	: Interleaver<T>(size, 1, name), seed(seed) { gen_lookup_tables(); }

protected:
	void gen_lookup_tables()
	{
		for (auto i = 0; i < (int)this->pi.size(); i++)
			this->pi[i] = i;

		if (seed == -1)
		{
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(this->pi.begin(), this->pi.end(), g);
		}
		else
		{
			std::mt19937 rd_engine(seed);
			std::shuffle(this->pi.begin(), this->pi.end(), rd_engine);
		}

		for (auto i = 0; i < (int)this->pi_inv.size(); i++)
			this->pi_inv[this->pi[i]] = i;
	}
};

#endif	/* INTERLEAVER_RANDOM_HPP */

