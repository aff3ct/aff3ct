/*!
 * \file
 * \brief Class tools::BCH_polynomial_generator.
 */
#ifndef BCH_POLYNOMIAL_GENERATOR_HPP
#define BCH_POLYNOMIAL_GENERATOR_HPP

#include <vector>

#include "Tools/Math/Galois.hpp"

namespace aff3ct
{
namespace tools
{
template <typename I = int>
class BCH_polynomial_generator : public Galois<I>
{
protected:
	const int t;
	const int d;

	std::vector<I> g; // coefficients of the generator polynomial, g(x)

public:
	BCH_polynomial_generator(const int& N, const int& t, const std::vector<I> p = std::vector<I>());
	virtual ~BCH_polynomial_generator() = default;

	int get_d      () const;
	int get_t      () const; // get the correction power
	int get_n_rdncy() const; // get the number of redundancy bits

	const std::vector<I>& get_g() const; // get the coefficients of the generator polynomial

private:
	void compute_polynomial();
};
}
}

#endif /* BCH_POLYNOMIAL_GENERATOR_HPP */
