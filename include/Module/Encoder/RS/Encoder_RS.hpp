/*!
 * \file
 * \brief Class module::Encoder_RS.
 */
#ifndef ENCODER_RS_HPP_
#define ENCODER_RS_HPP_

#include <vector>

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Encoder/Encoder.hpp"

/* source : http://www.eccpage.com/  */

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RS : public Encoder<B>
{
public:
	using S = B; // symbol to represent data

protected:
	const int               K_rs, N_rs, m;// The RS size in symbols and the Galois Field size
	const int               n_rdncy_bits; // The number of redundancy bits
	const int               n_rdncy;      // number redundancy symbols
	const std::vector<int>& alpha_to;     // log table of GF(2**m)
	const std::vector<int>& index_of;     // antilog table of GF(2**m)
	const std::vector<int>& g;            // coefficients of the generator polynomial, g(x)
	std::vector<S>          bb;           // coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)
	std::vector<S>          packed_U_K;   // the source bits packed as GF(m) symbols
	std::vector<S>          packed_X_N;   // the encoded bits packed as GF(m) symbols

public:
	// K and N are the RS size in symbols: K_rs and N_rs. K and N in bits are deduced as K = K_rs * m and N = N_rs * M
	Encoder_RS(const int& K, const int& N, const tools::RS_polynomial_generator& GF);

	virtual ~Encoder_RS() = default;

	virtual Encoder_RS<B>* clone() const;

	bool is_codeword(const B *X_N);

protected:
	virtual void  _encode(const B *U_K, B *X_N, const size_t frame_id);
	virtual void __encode(const S *U_K, S *bb);
};
}
}

#endif // ENCODER_RS_HPP_
