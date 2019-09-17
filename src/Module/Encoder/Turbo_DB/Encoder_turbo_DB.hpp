#ifndef ENCODER_TURBO_DB_HPP_
#define ENCODER_TURBO_DB_HPP_

#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/RSC_DB/Encoder_RSC_DB.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_DB : public Encoder<B>
{
protected:
	const Interleaver<B> &pi;   // the interleaver

	Encoder_RSC_DB<B> &enco_n;  // encoder natural order
	Encoder_RSC_DB<B> &enco_i;  // encoder interleaved order

	std::vector<B> U_K_cpy;
	std::vector<B> U_K_i;       // internal buffer for the systematic bits in the interleaved domain
	std::vector<B> X_N_tmp;
	std::vector<B> par_n;       // internal buffer for the encoded    bits in the natural     domain
	std::vector<B> par_i;       // internal buffer for the encoded    bits in the interleaved domain

	bool is_codeword(const B *X_N);

public:
	Encoder_turbo_DB(const int& K, const int& N, const Interleaver<B> &pi,
	                 Encoder_RSC_DB<B> &enco_n, Encoder_RSC_DB<B> &enco_i);
	virtual ~Encoder_turbo_DB() = default;

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_TURBO_HPP_
