/*!
 * \file
 * \brief Class tools::Frozenbits_generator_5G.
 */
#ifndef FROZENBITS_GENERATOR_5G_HPP_
#define FROZENBITS_GENERATOR_5G_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_file.hpp"

namespace aff3ct
{
namespace tools
{
class Frozenbits_generator_5G : public Frozenbits_generator_file
{
private:
	const int m;
	const int N_max = 1024;

public:
	Frozenbits_generator_5G(const int K, const int N);

	~Frozenbits_generator_5G();

	virtual Frozenbits_generator_5G* clone() const;

private:
	void evaluate();

};
}
}

#endif /* FROZENBITS_GENERATOR_5G_HPP_ */
