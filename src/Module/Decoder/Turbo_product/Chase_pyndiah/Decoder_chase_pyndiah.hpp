#ifndef DECODER_CHASE_PYNDIAH_HPP_
#define	DECODER_CHASE_PYNDIAH_HPP_

#include <aff3ct.hpp>
#include <vector>

#include <chrono>

// #include "../Decoder_turbo_product.hpp"
#include "../../Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int, typename R = float>
class Decoder_chase_pyndiah : public Decoder_SISO_SIHO<B,R>
{
protected:

	struct info
	{
		R metric;
		int pos;
	};

	Decoder_HIHO<B> &dec;
	Encoder     <B> &enc;

	const int  N_np; // N without the parity bit: can be equal to N if there is no parity bit in the code
	const int  n_least_reliable_positions;
	const int  n_test_vectors;
	const int  n_competitors;
	const bool parity_extended;
	// bool parity_diff;

	std::vector<info> least_reliable_pos; // the list of least reliable positions
	std::vector<info> competitors;        // the competitors' metric and their related test vector position
	std::vector<B>    hard_Y_N;           // the taken hard decision at the input of the chase
	std::vector<B>    test_vect;          // the test vectors after being corrected by the decoder 'dec'
	std::vector<R>    metrics;            // the metrics of each test vector
	std::vector<bool> is_wrong;           // if true then the matching test vector is not a codeword
	std::vector<std::vector<bool>> tv_candidates; // the patterns of the least reliable position to flip

public:

	Decoder_chase_pyndiah(const int K, const int N, // N with the parity bit if any
	                      Decoder_HIHO<B> &dec,
	                      Encoder     <B> &enc,
	                      const int n_least_reliable_positions = 2,
	                      const int n_test_vectors = 0,
	                      const int n_competitors  = 0);

	void decode_siso   (const R *Y_N1, R *Y_N2); // size is length with parity bit if any
	void decode_siho   (const R *Y_N,  B *V_K );
	void decode_siho_cw(const R *Y_N,  B *V_N );

	const std::vector<uint32_t>& get_info_bits_pos();

protected:
	void decode_chase           (const R *Y_N);
	void find_least_reliable_pos(const R* Y_N);
	void compute_test_vectors   (            );
	void compute_metrics        (const R* Y_N);
	void compute_reliability    (const R* Y_N1, R* Y_N2);

	void bit_flipping(B* hard_vect, const int c);

	void generate_bit_flipping_candidates();
};

}
}

#endif	/* DECODER_CHASE_PYNDIAH_HPP_ */