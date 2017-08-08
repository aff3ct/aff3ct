#ifndef ENCODER_RA_HPP_
#define ENCODER_RA_HPP_

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RA : public Encoder<B>
{
protected:
	const int rep_count; // number of repetitions
	std::vector<B> U, tmp_X_N;

	const Interleaver<int>& interleaver;

public:
	Encoder_RA(const int& K, const int& N, const Interleaver<int>& interleaver, const int n_frames = 1,
	           const std::string name = "Encoder_RA");

	virtual ~Encoder_RA() {}

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_RA_HPP_
