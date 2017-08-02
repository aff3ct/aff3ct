#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>
#include <mipp.h>

#include "Tools/Math/Galois.hpp"

#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "../Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH : public Decoder_SIHO_HIHO<B,R>
{
private:
	Decoder_NO<B,R> hard_decision;

	std::vector<mipp::vector<int>> elp;
	mipp::vector<int> discrepancy;
	mipp::vector<int> l;
	mipp::vector<int> u_lu;
	mipp::vector<int> s;
	mipp::vector<int> loc;
	mipp::vector<int> reg;

protected:
	const int m;                // order of the Galois Field
	const int t;                // correction power
	const int d;                // minimum distance of the code (d=2t+1))

	mipp::vector<int> alpha_to; // log table of GF(2**m)
	mipp::vector<int> index_of; // antilog table of GF(2**m)

	mipp::vector<B> YH_N;       // hard decision input vector

public:
	Decoder_BCH(const int& K, const int& N, const int&t, const tools::Galois &GF,
	            const int n_frames = 1, const std::string name = "Decoder_BCH");
	virtual ~Decoder_BCH();

protected:
	void _decode     (      B *Y_N, B *V_K                    );
	void _decode_hiho(const B *Y_N, B *V_K, const int frame_id);
	void _decode_siho(const R *Y_N, B *V_K, const int frame_id);
};
}
}

#endif /* DECODER_BCH */
