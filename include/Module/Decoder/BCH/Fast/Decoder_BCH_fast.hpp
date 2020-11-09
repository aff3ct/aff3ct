/*!
 * \file
 * \brief Class module::Decoder_BCH_fast.
 */
#ifndef DECODER_BCH_FAST
#define DECODER_BCH_FAST

#include <vector>
#include <mipp.h>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH_fast : public Decoder_BCH<B,R>
{
private :
	const int t2;

protected:
	std::vector<B> YH_N; // hard decision input vector
	mipp::vector<mipp::Reg<B>> Y_N_reorderered;
	std::vector<mipp::vector<mipp::Reg<B>>> elp;
	mipp::vector<mipp::Reg<B>> discrepancy;
	mipp::vector<mipp::Reg<B>> l;
	mipp::vector<mipp::Reg<B>> u_lu;
	mipp::vector<mipp::Reg<B>> s;
	mipp::vector<mipp::Reg<B>> reg;

	const int m;               // order of the Galois Field
	const int d;               // minimum distance of the code (d=2t+1))

	const std::vector<B>& alpha_to; // log table of GF(2**m)
	const std::vector<B>& index_of; // antilog table of GF(2**m)

public:
	Decoder_BCH_fast(const int& K, const int& N, const tools::BCH_polynomial_generator<B> &GF);
	virtual ~Decoder_BCH_fast() = default;
	virtual Decoder_BCH_fast<B,R>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual int  _decode        (                      const size_t frame_id);
	virtual void _load          (const B *Y_N,         const size_t frame_id);
	virtual int  _decode_hiho   (const B *Y_N, B *V_K, const size_t frame_id);
	virtual int  _decode_hiho_cw(const B *Y_N, B *V_N, const size_t frame_id);
	virtual int  _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	virtual int  _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);
};
}
}

#endif /* DECODER_BCH_FAST */
