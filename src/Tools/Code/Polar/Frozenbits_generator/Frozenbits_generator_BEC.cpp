#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <numeric>
#include <cmath>


#include "Tools/Noise/Noise.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_BEC.hpp"

using namespace aff3ct::tools;

Frozenbits_generator_BEC
::Frozenbits_generator_BEC(const int K, const int N, const std::string &dump_channels_path, const bool dump_channels_single_thread)
: Frozenbits_generator(K, N, dump_channels_path, dump_channels_single_thread),
  m((int)std::log2(N)),
  z((int)std::exp2(m), 0)
{
}

Frozenbits_generator_BEC
::~Frozenbits_generator_BEC()
{
}

Frozenbits_generator_BEC* Frozenbits_generator_BEC
::clone() const
{
	auto t = new Frozenbits_generator_BEC(*this);
	return t;
}

void Frozenbits_generator_BEC
::evaluate()
{
	this->check_noise();

	std::iota(this->best_channels.begin(), this->best_channels.end(), 0);

	z[0] = std::log(static_cast<double>(this->noise->get_value()));

	for (auto l = 1; l <= m; l++)
	{
		auto b = exp2(l);
		auto stride = N / b;
		for (auto j = 0; j < b / 2; j++)
		{
			auto t = z[j * 2 * stride ];
			z[j * 2 * stride         ] = std::log(2) + t + std::log1p(- std::exp(2 * t - (std::log(2) + t)));
			z[j * 2 * stride + stride] = 2 * t;
		}
	}

	std::sort(this->best_channels.begin(), this->best_channels.end(), [this](int i1, int i2) { return z[i1] < z[i2]; });
}

void Frozenbits_generator_BEC
::check_noise()
{
	Frozenbits_generator::check_noise();

	this->noise->is_of_type_throw(tools::Noise_type::EP);
}