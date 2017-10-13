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

	std::vector<int> alpha_to; // log table of GF(2**m)
	std::vector<int> index_of; // antilog table of GF(2**m)
	std::vector<int> p;        // coefficients of a primitive polynomial used to generate GF(2**m)

public:
	Galois(const int& K, const int& N);
	virtual ~Galois();

	int get_K() const;
	int get_N() const;
	int get_m() const;

	const std::vector<int>& get_alpha_to() const;
	const std::vector<int>& get_index_of() const;
	const std::vector<int>& get_p       () const;

private:
	void Select_Polynomial();
	void Generate_GF();
};
}
}

#endif /* GALOIS_HPP */
