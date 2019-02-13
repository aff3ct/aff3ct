#ifndef FROZENBITS_GENERATOR_GA_HPP_
#define FROZENBITS_GENERATOR_GA_HPP_

#include <limits>
#include <vector>

#include "Frozenbits_generator.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_GA : public Frozenbits_generator
{
private:
	const int m;
	std::vector<double> z;

	const double alpha = -0.4527;
	const double beta  =  0.0218;
	const double gamma =  0.8600;

	const double a =  1.0  / alpha;
	const double b = -beta / alpha;
	const double c =  1.0  / gamma;

	const double phi_pivot     = 0.867861;
	const double phi_inv_pivot = 0.6845772418;

	const double bisection_max = std::numeric_limits<double>::max();

public:
	Frozenbits_generator_GA(const int K, const int N);

	virtual ~Frozenbits_generator_GA() = default;

protected:
	void   evaluate();
	double phi    (double t);
	double phi_inv(double t);
	virtual void check_noise();
};
}
}

#endif /* FROZENBITS_GENERATOR_GA_HPP_ */
