#ifndef DECODER_TURBO_PRODUCT_HPP_
#define DECODER_TURBO_PRODUCT_HPP_

#include "Module/Interleaver/Interleaver.hpp"

#include "../Decoder_SISO_SIHO.hpp"
#include "../Decoder_HIHO.hpp"
#include "../../Encoder/Encoder.hpp"

#include "Chase_pyndiah/Decoder_chase_pyndiah.hpp"

namespace aff3ct
{
namespace module
{

/*
 * Iteration column by column and then row by row, calling respectively cp_c and cp_r.
 * Compute LLR iteration i:
 *  - for columns:
 *        Ri = Wi * alpha[2 * i + 0] + C
 *     with Wi the results of the Chase Pyndiah decoder 'cp_c' on R(i-1) and C the input LLR from the demodulator
 *     when 'beta' vector is given then set the beta value of 'cp_c' to beta[2 * i + 0]
 *  - for rows:
 *     Ri = Wi * alpha[2 * i + 1] + C
 *     with Wi the results of the Chase Pyndiah decoder 'cp_r' on R(i-1) and C the input LLR from the demodulator
 *     when 'beta' vector is given then set the beta value of 'cp_c' to beta[2 * i + 1]
 *
 */
template <typename B = int, typename R = float>
class Decoder_turbo_product : public Decoder_SISO_SIHO<B,R>
{
protected:
	const int  n_ite; // number of iterations
	const std::vector<float> alpha;
	const std::vector<float> beta;

	const Interleaver<R> &pi;
	Decoder_chase_pyndiah<B,R> &cp_r; // row decoder
	Decoder_chase_pyndiah<B,R> &cp_c; // col decoder

	std::vector<R> Y_N_i;
	std::vector<R> Y_N_pi;
	std::vector<B> V_K_i;
	std::vector<B> V_N_i;

	std::vector<R> Y_N_cha_i;

public:
	Decoder_turbo_product(const int& n_ite,
	                      const std::vector<float>& alpha,
	                      const Interleaver<R> &pi,
	                      Decoder_chase_pyndiah<B,R> &cp_r,
	                      Decoder_chase_pyndiah<B,R> &cp_c,
	                      const std::vector<float>& beta = {},
	                      const int n_frames = 1);
	virtual ~Decoder_turbo_product() = default;

protected:
	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	// when return_K_siso = 0 then hard decode and fill V_K_i
	// else if = 1 then hard decode and fill V_H_i
	// else soft decode and fill Y_N_i
	virtual void _decode(const R *Y_N, int return_K_siso);

};

}
}

#endif /* DECODER_TURBO_PRODUCT_HPP_ */
