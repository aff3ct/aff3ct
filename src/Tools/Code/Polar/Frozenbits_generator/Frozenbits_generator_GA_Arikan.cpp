#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include "Tools/general_utils.h"
#endif
#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <streampu.hpp>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_GA_Arikan.hpp"
#include "Tools/Noise/Noise.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Frozenbits_generator_GA_Arikan ::Frozenbits_generator_GA_Arikan(const int K,
                                                                const int N,
                                                                const std::string& dump_channels_path,
                                                                const bool dump_channels_single_thread)
  : Frozenbits_generator(K, N, dump_channels_path, dump_channels_single_thread)
  , m((int)std::log2(N))
  , z((int)std::exp2(m), 0)
{
}

Frozenbits_generator_GA_Arikan*
Frozenbits_generator_GA_Arikan ::clone() const
{
    auto t = new Frozenbits_generator_GA_Arikan(*this);
    return t;
}

void
Frozenbits_generator_GA_Arikan::generate_error_probability(std::vector<double>& P_err, double _snr)
{
    if (P_err.size() != (unsigned)N)
    {
        std::stringstream message;
        message << "'P_err.size()' has to be equal to 'N' ('P_err.size()' = " << P_err.size() << ", 'N' = " << N
                << ").";
        throw spu::tools::length_error(__FILE__, __LINE__, __func__, message.str());
    }

    double _noise = tools::ebn0_to_esn0(_snr, (double)this->K / (double)this->N, 1);
    _noise = tools::esn0_to_sigma(_noise, 1);
    for (auto i = 0; i < std::exp2(m); i++)
        z[i] = 2.0 / std::pow(_noise, 2.0);

    this->intermediate_evaluate();

    // compute the error probability for each bit channel
    for (auto i = 0; i < std::exp2(m); i++)
    {
        double T = z[i];
        P_err[i] = 0.5 * std::erfc(0.5 * std::sqrt(T));
    }
    // std::copy(this->z.begin(), this->z.end(), P_err.begin());
}

void
Frozenbits_generator_GA_Arikan ::evaluate()
{
    this->check_noise();

    for (auto i = 0; i < std::exp2(m); i++)
        z[i] = 2.0 / std::pow((double)this->noise->get_value(), 2.0);

    this->intermediate_evaluate();

    std::iota(this->best_channels.begin(), this->best_channels.end(), 0);
    std::sort(this->best_channels.begin(), this->best_channels.end(), [this](int i1, int i2) { return z[i1] > z[i2]; });
}

void
Frozenbits_generator_GA_Arikan::intermediate_evaluate()
{
    for (auto l = 1; l <= m; l++)
    {
        auto o1 = (int)std::exp2(m - l + 1);
        auto o2 = (int)std::exp2(m - l);

        for (auto t = 0; t < (int)std::exp2(l - 1); t++)
        {
            double T = z[t * o1];

            z[t * o1] = phi_inv(1.0 - std::pow(1.0 - phi(T), 2.0));
            if (z[t * o1] == HUGE_VAL) z[t * o1] = T + M_LN2 / (alpha * gamma);

            z[t * o1 + o2] = 2.0 * T;
        }
    }
}

double
Frozenbits_generator_GA_Arikan ::phi(double t)
{
    if (t < phi_pivot)
        return std::exp(0.0564 * t * t - 0.48560 * t);
    else // if(t >= phi_pivot)
        return std::exp(alpha * std::pow(t, gamma) + beta);
}

double
Frozenbits_generator_GA_Arikan ::phi_inv(double t)
{
    if (t > phi_inv_pivot)
        return 4.304964539 * (1 - sqrt(1 + 0.9567131408 * std::log(t)));
    else
        return std::pow(a * std::log(t) + b, c);
}

void
Frozenbits_generator_GA_Arikan ::check_noise()
{
    Frozenbits_generator::check_noise();

    this->noise->is_of_type_throw(tools::Noise_type::SIGMA);
}
