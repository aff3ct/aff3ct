#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <limits>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Tools/Noise/Noise.hpp>

#include "Frozenbits_generator_BEC.hpp"

using namespace aff3ct::tools;

Frozenbits_generator_BEC
::Frozenbits_generator_BEC(const int K, const int N)
: Frozenbits_generator(K, N), m((int)std::log2(N)), z((int)std::exp2(m), 0)
{
}

Frozenbits_generator_BEC
::~Frozenbits_generator_BEC()
{
}

void Frozenbits_generator_BEC
::evaluate()
{
	for (unsigned i = 0; i != this->best_channels.size(); i++) 
		this->best_channels[i] = i;

	for (auto i = 0; i < std::exp2(m); i++)
		z[i] = 2.0 / std::pow((double)this->n->get_noise(), 2.0);

	for (auto l = 1; l <= m; l++)
	{
		auto o1 = (int)std::exp2(m - l +1);
		auto o2 = (int)std::exp2(m - l   );

//		for (auto t = 0; t < (int)std::exp2(l -1); t++)
//		{
//			double T = z[t * o1];
//
//			z[t * o1] = phi_inv(1.0 - std::pow(1.0 - phi(T), 2.0));
//			if (z[t * o1] == HUGE_VAL)
//				z[t * o1] = T + M_LN2 / (alpha * gamma);
//
//			z[t * o1 + o2] = 2.0 * T;
//		}
	}

	std::sort(this->best_channels.begin(), this->best_channels.end(), [this](int i1, int i2) { return z[i1] > z[i2]; });
}

void Frozenbits_generator_BEC
::check_noise()
{
	Frozenbits_generator::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}