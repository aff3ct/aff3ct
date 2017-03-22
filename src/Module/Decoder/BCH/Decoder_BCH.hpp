#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/Algebraic/Galois.hpp"

#include "../Decoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
class Decoder_BCH : public Decoder<B, R>
{
private:
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
	mipp::vector<B> V_K;

public:
	Decoder_BCH(const int& K, const int& N, const int& m, const int&t, const tools::Galois &GF,
	            const int n_frames = 1, const std::string name = "Decoder_BCH");
	virtual ~Decoder_BCH();

protected:
	void load        (const mipp::vector<R>& Y_N);
	void _hard_decode(                          );
	void store       (      mipp::vector<B>& V_K) const;
};
}
}

#endif /* DECODER_BCH */
