#ifndef DECODER_CHASE_PYNDIAH_HPP_
#define	DECODER_CHASE_PYNDIAH_HPP_

#include <aff3ct.hpp>
#include <vector>

#include "../Decoder_turbo_product_code.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int, typename R = float>
class Decoder_chase_pyndiah : public Decoder_turbo_product_code<B,R>
{
protected:

	struct info
	{
		R metric;
		int pos;
	};

	const R   alpha;
	const int n_least_reliable_positions;

	std::vector<info> least_reliable_pos;
	std::vector<B>    hard_Rprime;
	B parity_diff;

	const int n_test_vectors;
	std::vector<B> test_vect;
	std::vector<R> metrics;

	const int n_competitors;
	std::vector<info> competitors; // map of the competitor metric and its related test vector position

public:

	Decoder_chase_pyndiah(const int& n_ite,
	                      const Interleaver<R> &pi,
	                      Decoder_HIHO<B> &hiho_n,
	                      Decoder_HIHO<B> &hiho_i,
	                      const R   alpha = 0.5,
	                      const int n_least_reliable_positions = 2,
	                      const int n_competitors = 0);

protected:
	void _decode(const R *Y_N, int return_K_siso); // return_K_siso = 0 then hard decode and fill V_K_i else if = 1 then hard decode and fill V_H_i else soft decode and fill nothing

	void _decode_row_siso(const R *R_cha, const R *R_prime, R *R_dec, Decoder_HIHO<B> &hiho, const int size); // size is length with parity bit if any
	void _decode_row_siho(const R *R_cha, const R *R_prime, B *R_dec, Decoder_HIHO<B> &hiho, const int size, const bool return_K = true);

	bool _decode_chase          (const R *R_prime, Decoder_HIHO<B> &hiho,    const int size);
	B    compute_parity         (const B* data,                              const int size);
	void find_least_reliable_pos(const R* R_prime,                           const int size);
	void compute_test_vectors   (Decoder_HIHO<B> &hiho,                      const int size);
	void compute_metrics        (const R* R_prime,                           const int size);
	void compute_reliability    (const R* R_cha, const R* R_prime, R* R_dec, const int size);

	void apply_candidate(B* hard_vect, const int c);
};

}
}

#endif	/* DECODER_CHASE_PYNDIAH_HPP_ */