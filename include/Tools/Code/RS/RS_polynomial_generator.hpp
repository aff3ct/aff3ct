/*!
 * \file
 * \brief Class tools::RS_polynomial_generator.
 */
#ifndef RS_POLYNOMIAL_GENERATOR_HPP
#define RS_POLYNOMIAL_GENERATOR_HPP

#include <vector>

#include "Tools/Math/Galois.hpp"

/* source : http://www.eccpage.com/  */

namespace aff3ct
{
namespace tools
{
class RS_polynomial_generator : public Galois<int>
{
protected:
	const int t;
	const int d;

	std::vector<int> g; // coefficients of the generator polynomial, g(x)

public:
	RS_polynomial_generator(const int& N, const int& t);
	virtual ~RS_polynomial_generator() = default;

	int get_d      () const;
	int get_t      () const; // get the correction power
	int get_n_rdncy() const; // get the number of redundancy bits

	const std::vector<int>& get_g() const; // get the coefficients of the generator polynomial

private:
	void compute_polynomial();
};
}
}

#endif /* RS_POLYNOMIAL_GENERATOR_HPP */
