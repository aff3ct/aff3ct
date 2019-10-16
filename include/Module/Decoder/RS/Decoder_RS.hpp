#ifndef DECODER_RS
#define DECODER_RS

#include <vector>

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RS : public Decoder_SIHO_HIHO<B,R>
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

	Decoder_RS(const int K, const int N, const tools::RS_polynomial_generator &GF, const int n_frames = 1);

	virtual ~Decoder_RS() = default;

protected:
	virtual void _decode        (      S *Y_N,         const int frame_id) = 0;
	virtual void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	virtual void _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);
	virtual void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}

#endif /* DECODER_RS */
