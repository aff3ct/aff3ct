#include <limits>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Decoder_RSC_BCJR_intra.hpp"

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
                         const bool buffered_encoding,
                         const int n_frames,
                         const std::string name)
: Decoder_RSC_BCJR<B,R>(K, trellis, buffered_encoding, n_frames, 1, name),
  alpha(8 * (K +4) + 1 * mipp::nElReg<R>()),
  gamma(2 * (K +3) + 2 * mipp::nElReg<R>())
{
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
Decoder_RSC_BCJR_intra<B,R>
::~Decoder_RSC_BCJR_intra()
{
}

template <typename B, typename R>
void Decoder_RSC_BCJR_intra<B,R>
::soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext, const int n_frames)
{
	if (n_frames != -1 && n_frames <= 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const int real_n_frames = (n_frames != -1) ? n_frames : this->get_n_frames();

	if (real_n_frames != this->simd_inter_frame_level_siso)
	{
		std::stringstream message;
		message << "'real_n_frames' has to be equal to 'simd_inter_frame_level_siso' ('real_n_frames' = "
		        << real_n_frames << ", 'simd_inter_frame_level_siso' = " << this->simd_inter_frame_level_siso << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	const auto limit_size1 = (this->K + this->n_ff) * this->simd_inter_frame_level + mipp::nElReg<R>();

	if ((int)sys.size() < limit_size1)
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal or greater than 'limit_size1' ('sys.size()' = " << sys.size()
		        << ", 'limit_size1' = " << limit_size1 << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)par.size() < limit_size1)
	{
		std::stringstream message;
		message << "'par.size()' has to be equal or greater than 'limit_size1' ('par.size()' = " << par.size()
		        << ", 'limit_size1' = " << limit_size1 << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	const auto limit_size2 = this->K * this->simd_inter_frame_level + mipp::nElReg<R>();

	if ((int)ext.size() < limit_size2)
	{
		std::stringstream message;
		message << "'ext.size()' has to be equal or greater than 'limit_size2' * 'real_n_frames' ('ext.size()' = "
		        << ext.size() << ", 'limit_size2' = " << limit_size2 << ", 'real_n_frames' = " << real_n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	SISO<R>::soft_decode(sys.data(), par.data(), ext.data(), real_n_frames);
}

template <typename B, typename R>
void Decoder_RSC_BCJR_intra<B,R>
::_soft_decode(const R *sys, const R *par, R *ext, const int frame_id)
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
