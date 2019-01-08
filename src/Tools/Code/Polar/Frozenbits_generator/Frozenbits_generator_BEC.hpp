#ifndef FROZENBITS_GENERATOR_BEC_HPP_
#define FROZENBITS_GENERATOR_BEC_HPP_

#include <limits>
#include <vector>

#include "Frozenbits_generator.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_BEC : public Frozenbits_generator
{
private:
	const int m;
	std::vector<double> z;

public:
	Frozenbits_generator_BEC(const int K, const int N);

	~Frozenbits_generator_BEC();

protected:
	void   evaluate();
	double phi    (double t);
	double phi_inv(double t);
	virtual void check_noise();
};
}
}

#endif /* FROZENBITS_GENERATOR_BEC_HPP_ */
