#ifndef INTERLEAVER_CCSDS_HPP
#define	INTERLEAVER_CCSDS_HPP

#include <map>

#include "../Tools/bash_tools.h"

#include "Interleaver.hpp"

inline int pi_CCSDS(const int &index, const int &k_1, const int &k_2)
{
	constexpr int p[8] = {31, 37, 43, 47, 53, 59, 61, 67};

	auto m = index % 2;
	auto i = index / (2 * k_2);
	auto j = (index / 2) - i * k_2;
	auto t = (19 * i +1) % (k_1 / 2);
	auto q = t % 8;
	auto c = (p[q] * j + 21 * m) % k_2;

	return 2 * (t + c * (k_1 / 2) +1) - m -1;
}

template <typename T>
class Interleaver_CCSDS : public Interleaver<T>
{
public:
	Interleaver_CCSDS(int size, const std::string name = "Interleaver_CCSDS") 
	: Interleaver<T>(size, 1, name.c_str()) { gen_lookup_tables(); }

protected:
	void gen_lookup_tables()
	{
		std::map<T,T> k_1;
		std::map<T,T> k_2;
		k_1[1784] = 8; k_2[1784] = 223 * 1;
		k_1[3568] = 8; k_2[3568] = 223 * 2;
		k_1[7136] = 8; k_2[7136] = 223 * 4;
		k_1[8920] = 8; k_2[8920] = 223 * 5;
		
		int size = this->pi.size();
		if (k_1.find(size) != k_1.end())
		{
			for (auto i = 0; i < size; i++)
			{
				this->pi[i] = (T)pi_CCSDS(i, k_1[size], k_2[size]);
				assert(this->pi[i] >= 0 && this->pi[i] < size);
				this->pi_inv[this->pi[i]] = (T)i;
			}
		}
		else
		{
			std::cerr << bold_red("(EE) There is no CCSDS k_1 and k_2 parameters for size = ")
			          << bold_red(std::to_string(size)) 
			          << bold_red(" (supported size are K = {1784, 3568, 7136, 8920}. Exiting simulation.") 
			          << std::endl;
			exit(-1);
		}
	}
};

#endif	/* INTERLEAVER_CCSDS_HPP */