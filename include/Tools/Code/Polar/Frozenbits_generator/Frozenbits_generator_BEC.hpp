/*!
 * \file
 * \brief Class tools::Frozenbits_generator_BEC.
 */
#ifndef FROZENBITS_GENERATOR_BEC_HPP_
#define FROZENBITS_GENERATOR_BEC_HPP_

#include <string>
#include <vector>

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

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
	Frozenbits_generator_BEC(const int K, const int N, const std::string &dump_channels_path = "",
	                                                   const bool dump_channels_single_thread = true);

	~Frozenbits_generator_BEC();

	virtual Frozenbits_generator_BEC* clone() const;

protected:
	void   evaluate();
	double phi    (double t);
	double phi_inv(double t);
	virtual void check_noise();
};
}
}

#endif /* FROZENBITS_GENERATOR_BEC_HPP_ */
