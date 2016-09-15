#include <cmath>
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Frozenbits_generator_GA.hpp"

#ifdef _WIN32
#define M_PI 3.1415926535897932384626433832795
#endif

template <typename B>
Frozenbits_generator_GA<B>
::Frozenbits_generator_GA(const int K, const int N, const float sigma)
: Frozenbits_generator<B>(K, N, sigma), m(std::log2(N)), z(std::exp2(m), 0)
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
		z[i] = 4.0 / std::pow((double)this->sigma, 2.0);

	for (auto l = 1; l <= m; l++)
	{
		int o1 = std::exp2(m - l + 1);
		int o2 = std::exp2(m - l);

		for (auto t = 0; t < std::exp2(l - 1); t++)
		{
			double T = z[t * o1];
			z[t * o1     ] = (T < 140) ? phi_inv(1.0 - std::pow(1.0 - phi(T), 2.0)) : 
			                  T < 2810 ? phi_inv((2.0 - phi(T)) * phi(T)) : (T - 2.77);
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
		return std::exp(alpha * std::pow(t, gamma) + beta);
	else // if(t >= phi_pivot)
		return std::sqrt(M_PI / t) * std::exp(- t / 4.0) * (1.0 - 10.0 / (7.0 * t));
}

template <typename B>
double Frozenbits_generator_GA<B>
::phi_inv(double t)
{
	double x1, x2, phi_m;
	if (t >= phi_inv_pivot)	
	{
		return std::pow(a * std::log(t) + b, c);
	}
	else
	{
		x1 = 10.0;
		x2 = bisection_max;
		while(true)
		{
			phi_m = phi((x2 + x1) / 2.0);

			if (phi_m < t)
			{
				if ((t - phi_m) / t < epsilon)
					return (x2 + x1) / 2.0;
				else
					x2 = (x2 + x1) / 2.0;
			}
			else
			{
				if ((phi_m - t) / t < epsilon)
					return (x2 + x1) / 2.0;
				else
					x1 = (x2 + x1) / 2.0;
			}
		}
		return INFINITY;
	}
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
#endif
// ==================================================================================== explicit template instantiation
