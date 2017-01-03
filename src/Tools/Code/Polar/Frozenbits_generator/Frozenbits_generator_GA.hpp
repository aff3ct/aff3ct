#ifndef FROZENBITS_GENERATOR_GA_HPP_
#define FROZENBITS_GENERATOR_GA_HPP_

#include <limits>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Frozenbits_generator.hpp"

template <typename B>
class Frozenbits_generator_GA : public Frozenbits_generator<B>
{
private:
	const int m;
	mipp::vector<double> z;

	const double alpha = -0.4527;
	const double beta  =  0.0218;
	const double gamma =  0.8600;

	const double a =  1.0  / alpha;
	const double b = -beta / alpha;
	const double c =  1.0  / gamma;

	const double phi_pivot     = 0.867861;
	const double phi_inv_pivot = 0.6845772418;

	const double bisection_max = std::numeric_limits<double>::max();

	const double epsilon = 0.00000000001;

public:
	Frozenbits_generator_GA(const int K, const int N, const float sigma = 0.f);

	~Frozenbits_generator_GA();

protected:
	void   evaluate();
	double phi    (double t);
	double phi_inv(double t);
};

#endif /* FROZENBITS_GENERATOR_GA_HPP_ */
