#ifndef BCH_Polynomial_Generator_HPP
#define BCH_Polynomial_Generator_HPP

#include <vector>

#include "Tools/Math/Galois.hpp"

namespace aff3ct
{
namespace tools
{
class BCH_Polynomial_Generator : public Galois
{
protected:
	const int t;
	const int d;

	std::vector<int> g; // coefficients of the generator polynomial, g(x)

public:
	BCH_Polynomial_Generator(const int& K, const int& N, const int& t);
	virtual ~BCH_Polynomial_Generator();

	int get_d      () const;
	int get_t      () const; // get the correction power
	int get_n_rdncy() const; // get the number of redundancy bits

	const std::vector<int>& get_g() const; // get the coefficients of the generator polynomial

private:
	void compute_polynomial();
};
}
}

#endif /* GALOIS_HPP */
