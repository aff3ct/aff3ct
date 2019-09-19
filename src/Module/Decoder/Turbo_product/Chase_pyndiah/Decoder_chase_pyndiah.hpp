#ifndef DECODER_CHASE_PYNDIAH_HPP_
#define	DECODER_CHASE_PYNDIAH_HPP_

#include <cstdint>
#include <vector>

#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
/*
 * Chase :
 *   - take hard decision H on input R
 *   - create test vectors from test patterns after selecting the p least reliable positions -> give a p metric set Pm
 *   - hard decode with the HIHO decoder -> get the competitors C -> remove competitors that have not been corrected
 *   - compute the metrics Cm: euclidean distance of each competitor m compared to H
 *   - select the competitor with the smallest metric Dm -> get the decided word D
 *   - if SIHO, return D, if SISO, compute reliabilities of each bit of D -> Pyndiah
 * Pyndiah :
 *   - a, b, c, d and e are simulation constants changeable by the user
 *   - Compute extrinsic W as
 *          W = F - a * R
 *   - Compute the reliability F of D as, for each bit j of the word:
 *        - when Cj /= Dj in the competitor with the smallest metric Cm:
 *           Fj = Dj * [Cm - Dm] * b
 *        - when there is no such competitor as described above
 *           Fj = Dj * beta                        when set by user with 'set_beta' method, or
 *           Fj = Dj * [slm - c * Dm + d * |Rj|]   with 'slm' the sum from 0 to e of the least reliable metrics Pm
 *                                                 where 0 < e < p. If e == 0 then sum all Pm.
 *        with Dj =  1 when Hj = 0
 *                = -1 when Hj = 1
 */
template <typename B = int, typename R = float>
class Decoder_chase_pyndiah : public Decoder_SISO_SIHO<B,R>
{
protected:
	struct info
	{
		R metric;
		int pos;
	};

protected:
	Decoder_BCH<B,R> &dec;
	Encoder    <B  > &enc;

	const int  N_np; // N without the parity bit: can be equal to N if there is no parity bit in the code
	const int  n_least_reliable_positions;  // the number of least reliable positions to flip to create test vectors
	const int  n_test_vectors;              // the number of test vectors to really use from the 2^n_least_reliable_positions
	const int  n_competitors;               // the number of competitors to use from the test vectors
	      int  n_good_competitors;          // the number of competitors whom test vectors converged at hard decoding step
	const bool parity_extended;             // true if there is a parity bit
	// bool parity_diff;

	const std::vector<float> cp_coef; // the a, b, c, d and e coefficient described above in the class description

	std::vector<info> least_reliable_pos; // the list of least reliable positions
	std::vector<info> competitors;        // the competitors' metric and their related test vector position
	std::vector<B>    hard_Y_N;           // the taken hard decision at the input of the chase
	std::vector<B>    test_vect;          // the test vectors after being corrected by the decoder 'dec'
	std::vector<R>    metrics;            // the metrics of each test vector
	std::vector<bool> is_wrong;           // if true then the matching test vector is not a codeword
	std::vector<std::vector<bool>> test_patterns; // the patterns of the least reliable position to flip


	R beta;
	bool beta_is_set;

public:

	Decoder_chase_pyndiah(const int K, const int N, // N with the parity bit if any
	                      const int n_frames,
	                      Decoder_BCH<B,R> &dec,
	                      Encoder    <B  > &enc,
	                      const int n_least_reliable_positions = 2,
	                      const int n_test_vectors = 0,
	                      const int n_competitors  = 0,
	                      const std::vector<float>& cp_coef = {1,1,1,1,0}); // the a b c d and e coef

	virtual ~Decoder_chase_pyndiah() = default;


	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id); // size is length with parity bit if any
	void _decode_siho   (const R *Y_N,  B *V_K , const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N , const int frame_id);

	const std::vector<uint32_t>& get_info_bits_pos();

	// set the value of beta to use it in the reliability computation
	// when beta is clear, then compute it dynamically
	void set_beta(R b);
	void clear_beta();

protected:
	virtual void decode_chase           (const R *Y_N, const int frame_id);
	virtual void find_least_reliable_pos(const R* Y_N);
	virtual void compute_test_vectors   (              const int frame_id);
	virtual void compute_metrics        (const R* Y_N);
	virtual void compute_reliability    (const R* Y_N1, R* Y_N2);

	void bit_flipping(B* hard_vect, const int c);

	void generate_bit_flipping_candidates();
};

}
}

#endif	/* DECODER_CHASE_PYNDIAH_HPP_ */