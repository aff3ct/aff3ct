/*!
 * \file
 * \brief Class module::Decoder_RS.
 */
#ifndef DECODER_RS
#define DECODER_RS

#include <vector>

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RS : public Decoder_SIHO<B,R>
{
public:
	using S = B; // symbol to represent data

protected:
	const int K_rs, N_rs, m;          // The RS size in symbols and the Galois Field size
	const int n_rdncy_bits;           // The number of redundancy bits
	const int n_rdncy;                // number redundancy symbols
	const std::vector<int>& alpha_to; // log table of GF(2**m)
	const std::vector<int>& index_of; // antilog table of GF(2**m)
	const int t;                      // correction power
	const int N_p2_1;                 // the next power 2 of N_rs minus 1
	std::vector<S> YH_N;              // hard decision symbols input vector
	std::vector<B> YH_Nb;             // hard decision bits input vector

public:
	bool last_is_codeword;

	Decoder_RS(const int K, const int N, const tools::RS_polynomial_generator &GF);

	virtual ~Decoder_RS() = default;

	virtual Decoder_RS<B,R>* clone() const;

protected:
	virtual int _decode        (      S *Y_N,         const size_t frame_id) = 0;
	virtual int _decode_hiho   (const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	virtual int _decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
	virtual int _decode_siho   (const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	virtual int _decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
};
}
}

#endif /* DECODER_RS */
