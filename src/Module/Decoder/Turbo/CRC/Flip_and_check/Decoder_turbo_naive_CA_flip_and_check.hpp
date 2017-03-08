#ifndef DECODER_TURBO_NAIVE_CA_FLIP_AND_CHECK
#define DECODER_TURBO_NAIVE_CA_FLIP_AND_CHECK

#include "../Decoder_turbo_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_naive_CA_flip_and_check : public Decoder_turbo_naive_CA<B,R>
{
private:
	int q;
	int nb_patterns;
	mipp::vector<R> metric;
	mipp::vector<mipp::vector<B>> tab_flips; // contains all the binary masks (ie: 1000, 1100 ...)
	mipp::vector<B> fnc_ite;
	mipp::vector<B> s_tmp;

public:
	Decoder_turbo_naive_CA_flip_and_check(const int& K,
	                       const int& N_without_tb,
	                       const int& n_ite,
	                       const Interleaver<short> &pi,
	                       SISO<R> &siso_n,
	                       SISO<R> &siso_i,
	                       tools::Scaling_factor<R> &scaling_factor,
	                       CRC<B> &crc,
	                       int fnc_q,
	                       int fnc_m,
	                       int fnc_M,
	                       int fnc_s,
	                       const bool buffered_encoding = true);
	virtual ~Decoder_turbo_naive_CA_flip_and_check();

protected:
	virtual void _hard_decode();
	void apply_flip_and_check(bool& check_crc);

private:

};
}
}

#endif /*DECODER_TURBO_NAIVE_CA_FLIP_AND_CHECK*/
