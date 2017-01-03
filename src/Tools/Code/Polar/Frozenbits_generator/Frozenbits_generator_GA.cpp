#include <cmath>
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Frozenbits_generator_GA.hpp"

template <typename B>
Frozenbits_generator_GA<B>
::Frozenbits_generator_GA(const int K, const int N, const float sigma)
: Frozenbits_generator<B>(K, N, sigma), m((int)std::log2(N)), z((int)std::exp2(m), 0)
{
	std::cerr << bold_yellow("(WW) The GA (Gaussian Approximation) frozen bits generator is inaccurate.") 
	          << std::endl;
}

template <typename B>
Frozenbits_generator_GA<B>
::~Frozenbits_generator_GA()
{
}

template <typename B>
void Frozenbits_generator_GA<B>
::evaluate()
{
	for (unsigned i = 0; i != this->best_channels.size(); i++) 
		this->best_channels[i] = i;

	for (auto i = 0; i < std::exp2(m); i++)
		z[i] = 2.0 / std::pow((double)this->sigma, 2.0);

	for (auto l = 1; l <= m; l++)
	{
		auto o1 = (int)std::exp2(m - l + 1);
		auto o2 = (int)std::exp2(m - l);

		for (auto t = 0; t < (int)std::exp2(l - 1); t++)
		{
			double T = z[t * o1];
			
			z[t * o1     ] = phi_inv(1.0 - std::pow(1.0 - phi(T), 2.0));
			if (z[t * o1] == HUGE_VAL)
				z[t * o1] = T + M_LN2 / (alpha * gamma);

			z[t * o1 + o2] = 2.0 * T ;
		}
	}

	std::sort(this->best_channels.begin(), this->best_channels.end(), [this](int i1, int i2) { return z[i1] > z[i2]; });
}

template <typename B>
double Frozenbits_generator_GA<B>
::phi(double t)
{
	if (t < phi_pivot)
		return std::exp(0.0564 * t * t - 0.48560 * t);
	else // if(t >= phi_pivot)
		return std::exp(alpha * std::pow(t, gamma) + beta);
}

template <typename B>
double Frozenbits_generator_GA<B>
::phi_inv(double t)
{
	if (t > phi_inv_pivot)	
		return 4.304964539 * (1 - sqrt(1 + 0.9567131408*std::log(t)));
	else
		return std::pow(a * std::log(t) + b, c);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Frozenbits_generator_GA<B_8>;
template class Frozenbits_generator_GA<B_16>;
template class Frozenbits_generator_GA<B_32>;
template class Frozenbits_generator_GA<B_64>;
#else
template class Frozenbits_generator_GA<B>;
#ifndef PREC_32_BIT
template class Frozenbits_generator_GA<B_32>;
#endif
#endif
// ==================================================================================== explicit template instantiation
