#ifndef DECODER_RA
#define DECODER_RA

#include <vector>
#include <mipp.h>

#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "../Decoder_SIHO.hpp"
#include "../../Interleaver/Interleaver.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RA : public Decoder_SIHO<B,R>
{
private:
	Decoder_NO<B,R> hard_decision;

protected:
	const int rep_count; // number of repetitions
	int max_iter;        // max number of iterations

	mipp::vector<R> Fw, Bw;
	mipp::vector<R> Tu, Td, Wu, Wd, U;
	std::vector<mipp::vector<R>> Xd, Xu;

	const Interleaver<int>& interleaver;

public:
	Decoder_RA(const int& K, const int& N, const Interleaver<int>& interleaver, int max_iter,
	           const int n_frames = 1, const std::string name = "Decoder_RA");
	virtual ~Decoder_RA();

protected:
	void _decode_siho(const R *Y_N, B *V_K, const int frame_id);

private:
	R check_node(R a, R b);
	int sign(R x);
};
}
}

#endif /* DECODER_RA */
