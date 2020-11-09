/*!
 * \file
 * \brief Class module::Encoder_BCH.
 */
#ifndef ENCODER_BCH_HPP_
#define ENCODER_BCH_HPP_

#include <vector>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_BCH : public Encoder<B>
{

protected:
	const int n_rdncy;       // number redundancy bits
	const std::vector<B> &g; // coefficients of the generator polynomial, g(x)
	std::vector<B> bb;       // coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)

public:
	Encoder_BCH(const int& K, const int& N, const tools::BCH_polynomial_generator<B>& GF);

	virtual ~Encoder_BCH() = default;

	virtual Encoder_BCH<B>* clone() const;

	bool is_codeword(const B *X_N);

protected:
	virtual void  _encode(const B *U_K, B *X_N, const size_t frame_id);
	virtual void __encode(const B *U_K, B *bb);
};
}
}

#endif // ENCODER_BCH_HPP_
