/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_inter_std.
 */
#ifndef DECODER_RSC_BCJR_INTER_STD_HPP_
#define DECODER_RSC_BCJR_INTER_STD_HPP_

#include <vector>

#include "Tools/Math/max.h"
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, tools::proto_max_i<R> MAX = tools::max_i>
class Decoder_RSC_BCJR_inter_std : public Decoder_RSC_BCJR_inter<B,R>
{
public:
	Decoder_RSC_BCJR_inter_std(const int &K,
	                           const std::vector<std::vector<int>> &trellis,
	                           const bool buffered_encoding = true,
	                           const int n_frames = 1);
	virtual ~Decoder_RSC_BCJR_inter_std() = default;

#ifdef _MSC_VER // Windows with MSVC
	virtual Decoder_RSC_BCJR_inter<B,R>* clone() const;
#else
	virtual Decoder_RSC_BCJR_inter_std<B,R,MAX>* clone() const;
#endif

protected:
	void _decode_siso(const R *sys, const R *par, R *ext, const int frame_id);

	void compute_gamma(const R *sys, const R *par);
	void compute_alpha(                          );
	void compute_beta (                          );
	void compute_ext  (const R *sys,       R *ext);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hxx"
#endif

#endif /* DECODER_RSC_BCJR_INTER_STD_HPP_ */
