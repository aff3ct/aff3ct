#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>

#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH : public Decoder_SIHO_HIHO<B,R>
{
protected:
	const int t; // correction power
	const int N_p2_1; // the next power 2 of N minus 1
	std::vector<B> last_is_codeword; // a value per frame

public:
	Decoder_BCH(const int K, const int N, const int t, const int n_frames = 1);

	virtual ~Decoder_BCH() = default;

	bool get_last_is_codeword(const int frame_id = -1) const;
};
}
}

#endif /* DECODER_BCH */
