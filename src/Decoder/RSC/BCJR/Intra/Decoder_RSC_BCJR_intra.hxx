#include <limits>

#include "Decoder_RSC_BCJR_intra.hpp"

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
::Decoder_RSC_BCJR_intra(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, buffered_encoding),
  alpha(8 * (K +4) + 1 * mipp::nElReg<R>()),
  gamma(2 * (K +3) + 2 * mipp::nElReg<R>())
{
	// init alpha values
	RSC_BCJR_intra_init<R>::apply(alpha);
}

template <typename B, typename R>
Decoder_RSC_BCJR_intra<B,R>
::~Decoder_RSC_BCJR_intra()
{
}

template <typename B, typename R>
void Decoder_RSC_BCJR_intra<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	this->compute_beta_ext(sys, ext);
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_intra_div_or_not
{
	static mipp::Reg<R> apply(mipp::Reg<R> r)
	{
		return r;
	}
};

template <>
struct RSC_BCJR_intra_div_or_not <signed char>
{
	static mipp::Reg<signed char> apply(mipp::Reg<signed char> r)
	{
		return mipp::div2(r);
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_intra_post
{
	static mipp::Reg<R> compute(const mipp::Reg<R> &r_post)
	{
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