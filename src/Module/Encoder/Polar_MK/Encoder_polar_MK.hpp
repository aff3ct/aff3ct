#ifndef ENCODER_POLAR_MK_HPP_
#define ENCODER_POLAR_MK_HPP_

#include <vector>

#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
#include "Tools/Algo/Matrix/Full_matrix/Full_matrix.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar_MK : public Encoder<B>, public tools::Frozenbits_notifier
{
protected:
	const int                              b;           // base power
	const int                              m;           // log_b of code length
	const std::vector<bool>&               frozen_bits; // true means frozen, false means set to 0/1
	const std::vector<std::vector<bool>>   kernel_matrix;
	      std::vector<B>                   X_N_tmp;
	      std::vector<std::vector<bool>>   G;
	      std::vector<std::vector<int8_t>> G_trans;
	      std::vector<int8_t>              tmp1;
	      std::vector<int8_t>              tmp2;
	      std::vector<int8_t>              Ke;

public:
	Encoder_polar_MK(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                 const std::vector<std::vector<bool>>& kernel_matrix = {{1,0},{1,1}}, const int n_frames = 1);
	virtual ~Encoder_polar_MK() = default;

	// bool is_codeword(const B *X_N);

	virtual void notify_frozenbits_update();

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
	void convert(const B *U_K, int8_t *U_N);
	void light_encode(const int8_t *in, int8_t *out);
};
}
}

#endif // ENCODER_POLAR_MK_HPP_
