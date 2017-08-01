#ifndef ENCODER_TURBO_DB_HPP_
#define ENCODER_TURBO_DB_HPP_

#include <string>
#include <vector>
#include <mipp.h>

#include "Module/Interleaver/Interleaver.hpp"

#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_DB : public Encoder<B>
{
protected:
	const Interleaver<int> &pi; // the interleaver

	Encoder_RSC_DB<B> &enco_n;  // encoder natural order
	Encoder_RSC_DB<B> &enco_i;  // encoder interleaved order

	mipp::vector<B> U_K_i;      // internal buffer for the systematic bits in the interleaved domain
	mipp::vector<B> par_n;      // internal buffer for the encoded    bits in the natural     domain
	mipp::vector<B> par_i;      // internal buffer for the encoded    bits in the interleaved domain

public:
	Encoder_turbo_DB(const int& K, const int& N, const Interleaver<int> &pi,
	                 Encoder_RSC_DB<B> &enco_n, Encoder_RSC_DB<B> &enco_i, const int n_frames = 1,
	                 const std::string name = "Encoder_turbo_DB");
	virtual ~Encoder_turbo_DB() {}

	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_TURBO_HPP_
