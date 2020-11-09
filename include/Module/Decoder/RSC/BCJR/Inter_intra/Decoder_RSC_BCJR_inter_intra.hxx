#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Decoder/RSC/BCJR/Inter_intra/Decoder_RSC_BCJR_inter_intra.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R>
Decoder_RSC_BCJR_inter_intra<B,R>
::Decoder_RSC_BCJR_inter_intra(const int &K,
                               const std::vector<std::vector<int>> &trellis,
                               const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, trellis, buffered_encoding),
  alpha(8 * (K +4) * (mipp::N<R>()/8) + 1 * mipp::N<R>()),
  gamma(2 * (K +3) * (mipp::N<R>()/8) + 2 * mipp::N<R>())
{
	const std::string name = "Decoder_RSC_BCJR_inter_intra";
	this->set_name(name);
	this->set_n_frames_per_wave(mipp::N<R>() / 8);

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
}

template <typename B, typename R>
Decoder_RSC_BCJR_inter_intra<B,R>* Decoder_RSC_BCJR_inter_intra<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_RSC_BCJR_inter_intra<B,R>
::_load(const R *Y_N)
{
	if (this->buffered_encoding && this->get_n_frames_per_wave() > 1)
	{
		const auto tail = this->tail_length();

		constexpr auto n_frames = mipp::nElReg<R>() / 8;
		const auto frame_size = 2*this->K + tail;

		std::vector<const R*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N + f*frame_size;
		tools::Reorderer_static<R,n_frames>::apply(frames, this->sys.data(), this->K + tail/2);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N + f*frame_size + this->K + tail/2;
		tools::Reorderer_static<R,n_frames>::apply(frames, this->par.data(), this->K + tail/2);
	}
	else
		Decoder_RSC_BCJR<B,R>::_load(Y_N);
}

// template <typename B, typename R>
// int Decoder_RSC_BCJR_inter_intra<B,R>
// ::decode_siso(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext, const int n_frames)
// {
// 	if (n_frames != -1 && n_frames <= 0)
// 	{
// 		std::stringstream message;
// 		message << "'n_frames' has to be greater than 0 or equal to -1 ('n_frames' = " << n_frames << ").";
// 		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	const int real_n_frames = (n_frames != -1) ? n_frames : this->get_n_frames();

// 	if (real_n_frames != this->simd_inter_frame_level_siso)
// 	{
// 		std::stringstream message;
// 		message << "'real_n_frames' has to be equal to 'simd_inter_frame_level_siso' ('real_n_frames' = "
// 		        << real_n_frames << ", 'simd_inter_frame_level_siso' = " << this->simd_inter_frame_level_siso << ").";
// 		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	const auto limit_size1 = (this->K + this->n_ff) * this->simd_inter_frame_level + mipp::nElReg<R>();

// 	if ((int)sys.size() < limit_size1)
// 	{
// 		std::stringstream message;
// 		message << "'sys.size()' has to be equal or greater than 'limit_size1' ('sys.size()' = " << sys.size()
// 		        << ", 'limit_size1' = " << limit_size1 << ").";
// 		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	if ((int)par.size() < limit_size1)
// 	{
// 		std::stringstream message;
// 		message << "'par.size()' has to be equal or greater than 'limit_size1' ('par.size()' = " << par.size()
// 		        << ", 'limit_size1' = " << limit_size1 << ").";
// 		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	const auto limit_size2 = this->K * this->simd_inter_frame_level + mipp::nElReg<R>();

// 	if ((int)ext.size() < limit_size2)
// 	{
// 		std::stringstream message;
// 		message << "'ext.size()' has to be equal or greater than 'limit_size2' * 'real_n_frames' ('ext.size()' = "
// 		        << ext.size() << ", 'limit_size2' = " << limit_size2 << ", 'real_n_frames' = " << real_n_frames << ").";
// 		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
// 	}

// 	auto status = Decoder_SISO<R>::decode_siso(sys.data(), par.data(), ext.data(), real_n_frames);
// 	return status;
// }

template <typename B, typename R>
int Decoder_RSC_BCJR_inter_intra<B,R>
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

template <typename B, typename R>
void Decoder_RSC_BCJR_inter_intra<B,R>
::_store(B *V_K) const
{
	if (this->get_n_frames_per_wave() > 1)
	{
		constexpr auto n_frames = mipp::nElReg<B>() / 8;

		std::vector<B*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = V_K + f*this->K;
		tools::Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
	}
	else
		Decoder_RSC_BCJR<B,R>::_store(V_K);
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_inter_intra_div_or_not
{
	static mipp::Reg<R> apply(mipp::Reg<R> r)
	{
		return mipp::div2(r);
	}
};

template <>
struct RSC_BCJR_inter_intra_div_or_not <short>
{
	static mipp::Reg<short> apply(mipp::Reg<short> r)
	{
		// (WW) work only for max-log-MAP !!!
		return r;
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_inter_intra_post
{
	static mipp::Reg<R> compute(const mipp::Reg<R> &r_post)
	{
		return r_post;
	}
};

template <>
struct RSC_BCJR_inter_intra_post <short>
{
	static mipp::Reg<short> compute(const mipp::Reg<short> &r_post)
	{
		// (WW) work only for max-log-MAP !!!
		return mipp::div2(r_post);
	}
};

template <>
struct RSC_BCJR_inter_intra_post <signed char>
{
	static mipp::Reg<signed char> compute(const mipp::Reg<signed char> &r_post)
	{
		return r_post.sat(-63, 63);
	}
};

// ====================================================================================================== normalization
template <typename R>
struct RSC_BCJR_inter_intra_normalize_core
{
	static mipp::Reg<R> apply(const mipp::Reg<R> &r_metrics, const mipp::Reg<R> &r_cmask_norm)
	{
		const auto r_norm = r_metrics.shuff2(r_cmask_norm);
		return r_metrics - r_norm;
	}
};

template <typename R, int I = -1>
struct RSC_BCJR_inter_intra_normalize
{
	static mipp::Reg<R> apply(const mipp::Reg<R> &r_metrics, const mipp::Reg<R> &r_cmask_norm, const int &i = 0)
	{
		return r_metrics;
	}
};

template <>
struct RSC_BCJR_inter_intra_normalize <short, -1>
{
	static mipp::Reg<short> apply(const mipp::Reg<short> &r_metrics, const mipp::Reg<short> &r_cmask_norm, const int &i = 0)
	{
		if (i % 4 == 0)
			return RSC_BCJR_inter_intra_normalize_core<short>::apply(r_metrics, r_cmask_norm);
		else
			return r_metrics;
	}
};

template <>
struct RSC_BCJR_inter_intra_normalize <short, 3>
{
	static mipp::Reg<short> apply(const mipp::Reg<short> &r_metrics, const mipp::Reg<short> &r_cmask_norm, const int &i = 0)
	{
		return RSC_BCJR_inter_intra_normalize_core<short>::apply(r_metrics, r_cmask_norm);
	}
};

template <int I>
struct RSC_BCJR_inter_intra_normalize <signed char, I>
{
	static mipp::Reg<signed char> apply(const mipp::Reg<signed char> &r_metrics, const mipp::Reg<signed char> &r_cmask_norm, const int &i = 0)
	{
		return RSC_BCJR_inter_intra_normalize_core<signed char>::apply(r_metrics, r_cmask_norm);
	}
};
}
}
