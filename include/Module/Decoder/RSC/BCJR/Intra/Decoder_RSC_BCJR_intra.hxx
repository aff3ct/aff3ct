#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <string>
#include <limits>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Decoder/RSC/BCJR/Intra/Decoder_RSC_BCJR_intra.hpp"

namespace aff3ct
{
namespace module
{
template <typename R>
struct RSC_BCJR_intra_init
{
	static void apply(mipp::vector<R> &alpha)
	{
		// init alpha values
		alpha[0*8 +0] = (R)0;
		for (auto i = 1; i < 8; i++)
			alpha[0*8 +i] = -std::numeric_limits<R>::max();
	}
};

template <>
struct RSC_BCJR_intra_init<short>
{
	static void apply(mipp::vector<short> &alpha)
	{
		// init alpha values
		alpha[0*8 +0] = (short)0;
		for (auto i = 1; i < 8; i++)
			alpha[0*8 +i] = -(1 << (sizeof(short) * 8 -2));
	}
};

template <>
struct RSC_BCJR_intra_init<signed char>
{
	static void apply(mipp::vector<signed char> &alpha)
	{
		// init alpha values
		alpha[0*8 +0] = (signed char)0;
		for (auto i = 1; i < 8; i++)
			alpha[0*8 +i] = -63;
	}
};

template <typename B, typename R>
Decoder_RSC_BCJR_intra<B,R>
::Decoder_RSC_BCJR_intra(const int &K,
                         const std::vector<std::vector<int>> &trellis,
                         const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, trellis, buffered_encoding),
  alpha(8 * (K +4) + 1 * mipp::N<R>()),
  gamma(2 * (K +3) + 2 * mipp::N<R>())
{
	const std::string name = "Decoder_RSC_BCJR_intra";
	this->set_name(name);

	std::vector<std::vector<int>> req_trellis(10, std::vector<int>(8));
	req_trellis[0] = { 0,  2,  4,  6,  0,  2,  4,  6};
	req_trellis[1] = { 1, -1,  1, -1, -1,  1, -1,  1};
	req_trellis[2] = { 0,  1,  1,  0,  0,  1,  1,  0};
	req_trellis[3] = { 1,  3,  5,  7,  1,  3,  5,  7};
	req_trellis[4] = {-1,  1, -1,  1,  1, -1,  1, -1};
	req_trellis[5] = { 0,  1,  1,  0,  0,  1,  1,  0};
	req_trellis[6] = { 0,  4,  5,  1,  2,  6,  7,  3};
	req_trellis[7] = { 0,  0,  1,  1,  1,  1,  0,  0};
	req_trellis[8] = { 4,  0,  1,  5,  6,  2,  3,  7};
	req_trellis[9] = { 0,  0,  1,  1,  1,  1,  0,  0};

	for (unsigned i = 0; i < req_trellis.size(); i++)
		if (trellis[i] != req_trellis[i])
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Unsupported trellis.");

	// init alpha values
	RSC_BCJR_intra_init<R>::apply(alpha);
}

template <typename B, typename R>
Decoder_RSC_BCJR_intra<B,R>* Decoder_RSC_BCJR_intra<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_RSC_BCJR_intra<B,R>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	if (!mipp::isAligned(sys))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'sys' is misaligned memory.");

	if (!mipp::isAligned(par))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'par' is misaligned memory.");

	if (!mipp::isAligned(ext))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'ext' is misaligned memory.");

	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	this->compute_beta_ext(sys, ext);

	return 0;
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_intra_div_or_not
{
	static mipp::Reg<R> apply(mipp::Reg<R> r)
	{
		return mipp::div2(r);
	}
};

template <>
struct RSC_BCJR_intra_div_or_not <short>
{
	static mipp::Reg<short> apply(mipp::Reg<short> r)
	{
		// (WW) work only for max-log-MAP !!!
		return r;
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_intra_post
{
	static mipp::Reg<R> compute(const mipp::Reg<R> &r_post)
	{
		return r_post;
	}
};

template <>
struct RSC_BCJR_intra_post <short>
{
	static mipp::Reg<short> compute(const mipp::Reg<short> &r_post)
	{
		// (WW) work only for max-log-MAP !!!
		return mipp::div2(r_post);
	}
};

template <>
struct RSC_BCJR_intra_post <signed char>
{
	static mipp::Reg<signed char> compute(const mipp::Reg<signed char> &r_post)
	{
		return r_post.sat(-63, 63);
	}
};

// ====================================================================================================== normalization

template <typename R>
struct RSC_BCJR_intra_normalize_core
{
	static mipp::Reg<R> apply(const mipp::Reg<R> &r_metrics, const mipp::Reg<R> &r_cmask_norm)
	{
		const auto r_norm = r_metrics.shuff(r_cmask_norm);
		return r_metrics - r_norm;
	}
};

template <typename R, int I = -1>
struct RSC_BCJR_intra_normalize
{
	static mipp::Reg<R> apply(const mipp::Reg<R> &r_metrics, const mipp::Reg<R> &r_cmask_norm, const int &i = 0)
	{
		return r_metrics;
	}
};

template <>
struct RSC_BCJR_intra_normalize <short, -1>
{
	static mipp::Reg<short> apply(const mipp::Reg<short> &r_metrics, const mipp::Reg<short> &r_cmask_norm, const int &i = 0)
	{
		if (i % 4 == 0)
			return RSC_BCJR_intra_normalize_core<short>::apply(r_metrics, r_cmask_norm);
		else
			return r_metrics;
	}
};

template <>
struct RSC_BCJR_intra_normalize <short, 3>
{
	static mipp::Reg<short> apply(const mipp::Reg<short> &r_metrics, const mipp::Reg<short> &r_cmask_norm, const int &i = 0)
	{
		return RSC_BCJR_intra_normalize_core<short>::apply(r_metrics, r_cmask_norm);
	}
};
}
}
