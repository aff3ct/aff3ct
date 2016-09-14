#ifndef INTERLEAVER_GOLDEN_HPP
#define INTERLEAVER_GOLDEN_HPP

#include <algorithm>
#include <time.h>
#include <random>

#include "../Interleaver.hpp"

template <typename T>
class Interleaver_golden : public Interleaver<T>
{
public:
	Interleaver_golden(int size, const std::string name = "Interleaver_columns") 
	: Interleaver<T>(size, 1, name) { gen_lookup_tables(); }

protected:
	void gen_lookup_tables()
	{
		double g  = (sqrt(5)-1)/2.0;
		double g2 = g * g;
		double c  = this->pi.size() * g2;

		std::mt19937                           gen;
		std::uniform_real_distribution<double> dist(0.0, this->pi.size() * 0.1);

		std::vector<double> tab;

		for (size_t i=0; i < this->pi.size(); i++)
			tab.push_back(fmod(i*c + dist(gen),this->pi.size()));

		for (size_t i = 0; i < this->pi.size(); i++)
			this->pi[i] = i;

		std::sort(this->pi.begin(), this->pi.end(),[&tab](size_t i1, size_t i2) { return tab[i1] < tab[i2]; });

		for (size_t i = 0; i < this->pi_inv.size(); i++)
			this->pi_inv[this->pi[i]] = i;
	}
};

#endif  /* INTERLEAVER_GOLDEN_HPP */
