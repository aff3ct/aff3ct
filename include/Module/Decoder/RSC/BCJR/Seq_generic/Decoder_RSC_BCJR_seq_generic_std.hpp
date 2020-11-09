/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_seq_generic_std.
 */
#ifndef DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_
#define DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_

#include <vector>
#include <mipp.h>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename RD = float,
          tools::proto_max<R> MAX1 = tools::max, tools::proto_max<RD> MAX2 = tools::max>
class Decoder_RSC_BCJR_seq_generic_std : public Decoder_RSC_BCJR_seq_generic<B,R>
{
private:
	mipp::vector<R> beta_prev;
	mipp::vector<R> beta_cur;

public:
	Decoder_RSC_BCJR_seq_generic_std(const int &K,
	                                 const std::vector<std::vector<int>> &trellis,
	                                 const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR_seq_generic_std() = default;

	virtual Decoder_RSC_BCJR_seq_generic_std<B,R,RD,MAX1,MAX2>* clone() const;

protected:
	virtual int _decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id);
	virtual int _decode_siso    (const R *Y_N1, R *Y_N2, const size_t frame_id);

	virtual void compute_gamma   (const R *sys, const R *par    );
	virtual void compute_alpha   (                              );
	virtual void compute_beta    (                              );
	virtual void compute_ext     (const R *sys,       R *ext    );
	virtual void compute_beta_ext(const R *sys,       R *ext    );
	virtual void compute_ext_sys (const R *sys,       R *ext_sys);
	virtual void compute_ext_par (const R *par,       R *ext_par);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic_std.hxx"
#endif

#endif /* DECODER_RSC_BCJR_STD_GENERIC_SEQ_HPP_ */
