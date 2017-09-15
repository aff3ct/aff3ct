#ifndef GALOIS_HPP
#define GALOIS_HPP

#include <vector>

namespace aff3ct
{
namespace tools
{
class Galois
{
protected:
	const int K;
	const int N; // number of non-nul elements in the field : N = 2^m - 1
	const int m; // order of the Galois Field
	const int t;
	const int d;

	std::vector<int> alpha_to; // log table of GF(2**m)
	std::vector<int> index_of; // antilog table of GF(2**m)
	std::vector<int> p;        // coefficients of a primitive polynomial used to generate GF(2**m)
	std::vector<int> g;        // coefficients of the generator polynomial, g(x)

public:
	Galois(const int& K, const int& N, const int& t);
	virtual ~Galois();

	int get_K() const;
	int get_N() const;
	int get_m() const;
	int get_t() const;
	int get_d() const;

	const std::vector<int>& get_alpha_to() const;
	const std::vector<int>& get_index_of() const;
	const std::vector<int>& get_p       () const;
	const std::vector<int>& get_g       () const;

private:
	void Select_Polynomial();
	void Generate_GF();
	void Compute_BCH_Generator_Polynomial();
};
}
}

#endif /* GALOIS_HPP */
