#ifndef DECODER_TURBO_NAIVE_CA_SELF_CORRECTED
#define DECODER_TURBO_NAIVE_CA_SELF_CORRECTED

#include "../Decoder_turbo_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_naive_CA_self_corrected : public Decoder_turbo_naive_CA<B,R>
{
private:
	std::vector<mipp::vector<R>> ext_nat; // extrinsic coming from the SISO working in the natural domain. Data in the interleaved order
	std::vector<mipp::vector<R>> ext_int; // extrinsic coming from the SISO working in the interleaved domain. Data in the natural order
	mipp::vector<B> osc_nat;
	mipp::vector<B> osc_int;
	mipp::vector<B> previously_corrected_nat;
	mipp::vector<B> previously_corrected_int;

public:
	Decoder_turbo_naive_CA_self_corrected(const int& K,
	                       const int& N_without_tb,
	                       const int& n_ite,
	                       const Interleaver<short> &pi,
	                       SISO<R> &siso_n,
	                       SISO<R> &siso_i,
	                       tools::Scaling_factor<R> &scaling_factor,
	                       CRC<B> &crc,
	                       const bool buffered_encoding = true);
	virtual ~Decoder_turbo_naive_CA_self_corrected();

protected:
	virtual void _hard_decode();

private:
	void collect (const mipp::vector<R> &cur_ext, const int &ite, std::vector<mipp::vector<R>> &ext_hist);
	void calc_osc(const std::vector<mipp::vector<R>> &ext_hist, const int &ite, mipp::vector<B> &osc);
	void correct (const mipp::vector<B> &osc, mipp::vector<B> &prev_corr, mipp::vector<R> &ext);
};
}
}

#endif /*DECODER_TURBO_NAIVE_CA_SELF_CORRECTED*/
