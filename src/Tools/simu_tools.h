#ifndef SIMU_TOOLS_H_
#define SIMU_TOOLS_H_

#include <string>

#include "MIPP/mipp.h"

void check_errors(void *ptr, std::string str, const int tid = 0);

template <typename B>
void display_bit_vector(const mipp::vector<B> &vec)
{
	for(unsigned i = 0; i < vec.size(); i++)
		std::clog << std::setw(5) << i << "|";
	std::clog << std::endl;

	for(unsigned i = 0; i < vec.size(); i++)
		std::clog << std::setw(5) << ((vec[i] == 0) ? (int) 0 : (int) 1) << "|";
	std::clog << std::endl;
}

template <typename R>
void display_real_vector(const mipp::vector<R> &vec)
{
	for(unsigned i = 0; i < vec.size(); i++)
		std::clog << std::setw(5) << i << "|";
	std::clog << std::endl;

	for(unsigned i = 0; i < vec.size(); i++)
		if(vec[i] >= 0)
			std::clog << std::setprecision(2) << std::setw(5) << vec[i] << "|";
		else
			std::clog << std::setprecision(1) << std::setw(5) << vec[i] << "|";
	std::clog << std::endl;
}

template <typename Q>
void display_quantized_vector(const mipp::vector<Q> &vec)
{
	for(unsigned i = 0; i < vec.size(); i++)
		std::clog << std::setw(5) << i << "|";
	std::clog << std::endl;

	for(unsigned i = 0; i < vec.size(); i++)
		if(vec[i] >= 0)
			std::clog << std::setprecision(2) << std::setw(5) << (int)vec[i] << "|";
		else
			std::clog << std::setprecision(1) << std::setw(5) << (int)vec[i] << "|";
	std::clog << std::endl;
}

#endif /* SIMU_TOOLS_H_ */