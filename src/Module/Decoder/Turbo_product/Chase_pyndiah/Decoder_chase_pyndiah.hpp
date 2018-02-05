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
	const bool parity_extended;

	const int n_least_reliable_positions;

	std::vector<info> least_reliable_pos;
	std::vector<B>    hard_Rprime;
	B parity_diff;

	const int n_test_vectors;
	std::vector<B> test_vect;
	std::vector<R> metrics;
	std::vector<bool> is_wrong;

	const int n_competitors;
	std::vector<info> competitors; // map of the competitor metric and its related test vector position

	std::vector<std::vector<bool>> tv_candidates;

public:

	Decoder_chase_pyndiah(const int K, const int N, // N with the parity bit if any
	                      Decoder_HIHO<B> &dec,
	                      Encoder     <B> &enc,
	                      const int n_least_reliable_positions = 2,
	                      const int n_test_vectors = 0,
	                      const int n_competitors  = 0);

	void decode_siso   (const R *R_prime, R *R_dec); // size is length with parity bit if any
	void decode_siho   (const R *R_prime, B *R_dec);
	void decode_siho_cw(const R *R_prime, B *R_dec);

	const std::vector<uint32_t>& get_info_bits_pos();

protected:
	void decode_chase           (const R *R_prime);
	void find_least_reliable_pos(const R* R_prime);
	void compute_test_vectors   (                );
	void compute_metrics        (const R* R_prime);
	void compute_reliability    (const R* R_prime, R* R_dec);

	void bit_flipping(B* hard_vect, const int c);

	void generate_bit_flipping_candidates();
};

}
}

#endif	/* DECODER_CHASE_PYNDIAH_HPP_ */