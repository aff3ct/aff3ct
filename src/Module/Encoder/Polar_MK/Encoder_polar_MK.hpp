#ifndef ENCODER_POLAR_MK_HPP_
#define ENCODER_POLAR_MK_HPP_

#include <vector>

#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
#include "Tools/Algo/Matrix/Full_matrix/Full_matrix.hpp"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace tools
{
std::string display_kernel(const std::vector<std::vector<bool>>& pattern_bits);
}
}

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar_MK : public Encoder<B>, public tools::Frozenbits_notifier
{
protected:
	const std::vector<bool>&                          frozen_bits; // true means frozen, false means set to 0/1
	const std::vector<std::vector<std::vector<bool>>> kernel_matrices;
	const std::vector<uint32_t>                       stages;
//	      std::vector<B>                              X_N_tmp;
	      std::vector<std::vector<int8_t>>            Ke;
	      std::vector<uint32_t>                       idx;
	      std::vector<B>                              u;

public:
	Encoder_polar_MK(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                 const std::vector<std::vector<bool>>& kernel_matrix = {{1,0},{1,1}}, const int n_frames = 1);

	Encoder_polar_MK(const int& K, const int& N, const std::vector<bool>& frozen_bits,
	                 const std::vector<std::vector<std::vector<bool>>>& kernel_matrices,
	                 const std::vector<uint32_t> &stages, const int n_frames = 1);

	virtual ~Encoder_polar_MK() = default;

	void init();

	// bool is_codeword(const B *X_N);

	virtual void notify_frozenbits_update();

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
	void convert(const B *U_K, B *U_N);
	void light_encode(B *X_N);
};
}
}

#endif // ENCODER_POLAR_MK_HPP_
