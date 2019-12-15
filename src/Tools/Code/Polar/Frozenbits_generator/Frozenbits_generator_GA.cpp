#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <numeric>
#include <cmath>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

std::vector<bool> init_fb(const int K, const int N)
{
	std::vector<bool> fake_frozen_bits(N);
	std::fill(fake_frozen_bits.begin(),     fake_frozen_bits.begin() + K, 0);
	std::fill(fake_frozen_bits.begin() + K, fake_frozen_bits.end(),       1);
	return fake_frozen_bits;
}

Frozenbits_generator_GA
::Frozenbits_generator_GA(const int K, const int N, const Polar_code& code, const std::string &dump_channels_path,
                          const bool dump_channels_single_thread)
: Frozenbits_generator(K, N, dump_channels_path, dump_channels_single_thread),
  z(code.get_codeword_size()),
  fake_frozen_bits(init_fb(K, code.get_codeword_size())),
  decoder_sc(K, code.get_codeword_size(), code, fake_frozen_bits)
{
	recursive_override_frozen_bits(decoder_sc.polar_tree.get_root());

	for (size_t l = 0; l < decoder_sc.lambdas.size(); l++)
	{
		if (Polar_lambdas<int64_t,double>::functions.find(code.get_kernel_matrices()[l]) ==
		    Polar_lambdas<int64_t,double>::functions.end())
			throw runtime_error(__FILE__, __LINE__, __func__, "Unsupported polar kernel.");
		decoder_sc.lambdas[l] =
		    Polar_lambdas<int64_t,double,square_plus_DE>::functions[code.get_kernel_matrices()[l]];
	}
}

Frozenbits_generator_GA* Frozenbits_generator_GA
::clone() const
{
	auto t = new Frozenbits_generator_GA(*this);
	return t;
}

void Frozenbits_generator_GA
::evaluate()
{
	this->check_noise();

	std::fill(z.begin(), z.end(), 2.0 / std::pow((double)this->noise->get_value(), 2.0));

	this->decoder_sc._load(z.data());
	this->decoder_sc.recursive_decode(this->decoder_sc.polar_tree.get_root());
	this->recursive_store_DE(this->decoder_sc.polar_tree.get_root(), z.data());

	std::iota(this->best_channels.begin(), this->best_channels.end(), 0);
	std::sort(this->best_channels.begin(), this->best_channels.end(), [this](int i1, int i2) { return z[i1] > z[i2]; });
}

double Frozenbits_generator_GA
::phi(double t)
{
	if (t < phi_pivot)
		return std::exp(0.0564 * t * t - 0.48560 * t);
	else // if(t >= phi_pivot)
		return std::exp(alpha * std::pow(t, gamma) + beta);
}

double Frozenbits_generator_GA
::phi_inv(double t)
{
	if (t > phi_inv_pivot)
		return 4.304964539 * (1 - sqrt(1 + 0.9567131408 * std::log(t)));
	else
		return std::pow(a * std::log(t) + b, c);
}

void Frozenbits_generator_GA
::check_noise()
{
	Frozenbits_generator::check_noise();

	this->noise->is_of_type_throw(tools::Noise_type::SIGMA);
}

void Frozenbits_generator_GA
::recursive_override_frozen_bits(const Generic_node<module::Contents_MK_SC<int64_t, double>>* node_curr)
{
	if (!node_curr->is_leaf()) // stop condition
		for (auto c : node_curr->get_children())
			this->recursive_override_frozen_bits(c); // recursive call
	else
		node_curr->get_contents()->is_frozen_bit = true;
}

void Frozenbits_generator_GA
::recursive_store_DE(const Generic_node<module::Contents_MK_SC<int64_t, double>>* node_curr, double *z) const
{
	if (!node_curr->is_leaf()) // stop condition
		for (auto c : node_curr->get_children())
			this->recursive_store_DE(c, z); // recursive call
	else
		z[node_curr->get_lane_id()] = node_curr->get_contents()->l[0];
}

double Frozenbits_generator_GA
::square_plus_DE(const double& zl, const double& zr)
{
	auto z = phi_inv(1.0 - ((1.0 - phi(zl)) * (1.0 - phi(zr))));
	return (z == HUGE_VAL) ? zl + M_LN2 / (alpha * gamma) : z;
	// return z;
}