/*!
 * \file
 * \brief Class tools::Galois.
 */
#ifndef GALOIS_HPP
#define GALOIS_HPP

#include <vector>

namespace aff3ct
{
namespace tools
{
/*
 * \brief generate the Galois field for the given space size
 * \param I is the integral type used to stock alpha_to and index_of values
 */
template <typename I = int>
class Galois
{
protected:
	const int N; // number of non-nul elements in the field : N = 2^m - 1
	const int m; // order of the Galois Field

	std::vector<I> alpha_to; // log table of GF(2**m)
	std::vector<I> index_of; // antilog table of GF(2**m)
	std::vector<I> p;        // coefficients of a primitive polynomial used to generate GF(2**m)

public:
	explicit Galois(const int& N, const std::vector<I> p = {});
	virtual ~Galois() = default;

	int get_N() const;
	int get_m() const;

	const std::vector<I>& get_alpha_to() const;
	const std::vector<I>& get_index_of() const;
	const std::vector<I>& get_p       () const;

private:
	void select_polynomial();
	void generate_gf();
};
}
}

#endif /* GALOIS_HPP */
