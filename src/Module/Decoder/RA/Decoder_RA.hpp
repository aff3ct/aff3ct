#ifndef DECODER_RA
#define DECODER_RA

#include <vector>

#include "../Decoder_SIHO.hpp"
#include "../../Interleaver/Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RA : public Decoder_SIHO<B,R>
{
protected:
	const int rep_count; // number of repetitions
	int max_iter;        // max number of iterations

	std::vector<R> Fw, Bw;
	std::vector<R> Tu, Td, Wu, Wd, U;
	std::vector<mipp::vector<R>> Xd, Xu;

	const Interleaver<R>& interleaver;

public:
	Decoder_RA(const int& K, const int& N, const Interleaver<R>& interleaver, int max_iter, const int n_frames = 1);
	virtual ~Decoder_RA() = default;

protected:
	void _decode_siho(const R *Y_N, B *V_K, const int frame_id);

private:
	R check_node(R a, R b);
	int sign(R x);
};
}
}

#endif /* DECODER_RA */
