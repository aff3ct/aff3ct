#include <limits>

#include "Decoder_RSC_BCJR_inter_intra.hpp"

template <typename B, typename R>
Decoder_RSC_BCJR_inter_intra<B,R>
::Decoder_RSC_BCJR_inter_intra(const int &K, const mipp::vector<mipp::vector<int>> &trellis, const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, buffered_encoding, mipp::nElReg<R>() / 8),
  alpha(8 * (K +4) * (mipp::nElReg<R>()/8) + 1 * mipp::nElReg<R>()),
  gamma(2 * (K +3) * (mipp::nElReg<R>()/8) + 2 * mipp::nElReg<R>())
{
	mipp::vector<mipp::vector<int>> req_trellis(4);
	for (unsigned i = 0; i < req_trellis.size(); i++)
		req_trellis[i].resize(8);

	req_trellis[0] = {0,4,5,1,2,6,7,3};
	req_trellis[1] = {4,0,1,5,6,2,3,7};
	req_trellis[2] = {0,1,1,0,0,1,1,0};
	req_trellis[3] = {0,0,1,1,1,1,0,0};

	for (unsigned i = 0; i < req_trellis.size(); i++)
		if (trellis[i] != req_trellis[i])
		{
			std::cerr << "(EE) This decoder does not support the input trellis... Exiting." << std::endl;
			exit(-1);
		}
}

template <typename B, typename R>
Decoder_RSC_BCJR_inter_intra<B,R>
::~Decoder_RSC_BCJR_inter_intra()
{
}

template <typename B, typename R>
void Decoder_RSC_BCJR_inter_intra<B,R>
::load(const mipp::vector<R>& Y_N)
{
	if (this->buffered_encoding && this->get_n_frames() > 1)
	{
		const auto tail = this->tail_length();

		constexpr auto n_frames = mipp::nElReg<R>() / 8;
		const auto frame_size = 2*this->K + tail;

		std::vector<const R*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size;
		Reorderer_static<R,n_frames>::apply(frames, this->sys.data(), this->K);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size +this->K;
		Reorderer_static<R,n_frames>::apply(frames, this->par.data(), this->K);

		// tails bit
		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size + 2*this->K + tail/2;
		Reorderer_static<R,n_frames>::apply(frames, &this->sys[this->K*n_frames], tail/2);

		for (auto f = 0; f < n_frames; f++)
			frames[f] = Y_N.data() + f*frame_size + 2*this->K;
		Reorderer_static<R,n_frames>::apply(frames, &this->par[this->K*n_frames], tail/2);
	
	}
	else
		Decoder_RSC_BCJR<B,R>::load(Y_N);
}

template <typename B, typename R>
void Decoder_RSC_BCJR_inter_intra<B,R>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	this->compute_gamma   (sys, par);
	this->compute_alpha   (        );
	this->compute_beta_ext(sys, ext);
}

template <typename B, typename R>
void Decoder_RSC_BCJR_inter_intra<B,R>
::store(mipp::vector<B>& V_K) const
{
	if (this->get_n_frames() > 1)
	{
		constexpr auto n_frames = mipp::nElReg<B>() / 8;

		std::vector<B*> frames(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames[f] = V_K.data() + f*this->K;
		Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
	}
	else
		Decoder_RSC_BCJR<B,R>::store(V_K);
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_inter_intra_div_or_not
{
	static mipp::Reg<R> apply(mipp::Reg<R> r)
	{
		return r;
	}
};

template <>
struct RSC_BCJR_inter_intra_div_or_not <signed char>
{
	static mipp::Reg<signed char> apply(mipp::Reg<signed char> r)
	{
		return mipp::div2(r);
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_inter_intra_post
{
	static mipp::Reg<R> compute(const mipp::Reg<R> &r_post)
	{
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
		return r_metrics;
	}
};