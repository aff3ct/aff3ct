/*!
 * \file
 * \brief Class module::Decoder_SIHO_HIHO.
 */
#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>

#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH : public Decoder_SIHO<B,R>
{
protected:
	const int t; // correction power
	const int N_p2_1; // the next power 2 of N minus 1
	std::vector<B> last_is_codeword; // a value per frame

public:
	Decoder_BCH(const int K, const int N, const int t);

	virtual ~Decoder_BCH() = default;

	virtual Decoder_BCH<B,R>* clone() const;

	bool get_last_is_codeword(const int frame_id = -1) const;

	virtual void set_n_frames(const size_t n_frames);
};
}
}

#endif /* DECODER_BCH */
