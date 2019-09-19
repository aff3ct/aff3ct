#ifndef ENCODER_POLAR_HPP_
#define ENCODER_POLAR_HPP_

#include <vector>

#include "Tools/Code/Polar/Frozenbits_notifier.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_polar : public Encoder<B>, public tools::Frozenbits_notifier
{
protected:
	const int                m;           // log_2 of code length
	const std::vector<bool>& frozen_bits; // true means frozen, false means set to 0/1
	      std::vector<B>     X_N_tmp;

public:
	Encoder_polar(const int& K, const int& N, const std::vector<bool>& frozen_bits, const int n_frames = 1);
	virtual ~Encoder_polar() = default;

	void light_encode(B *bits);

	bool is_codeword(const B *X_N);

	virtual void notify_frozenbits_update();

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
	void convert(const B *U_K, B *U_N);
};
}
}

#endif // ENCODER_POLAR_HPP_
