#include "Decoder_RSC_BCJR_inter.hpp"

#include <limits>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/utils.h"

template <typename R>
struct RSC_BCJR_inter_init
{
	static void apply(mipp::vector<R> alpha[8], mipp::vector<R> beta[8], const int &K)
	{
		constexpr auto stride = mipp::nElmtsPerRegister<R>();

		// init alpha values
		for (auto j = 0; j < mipp::nElmtsPerRegister<R>(); j++)
		{
			alpha[0][j] = (R)0;
			for (auto i = 1; i < 8; i++)
				alpha[i][j] = -std::numeric_limits<R>::max();
			for (auto i = 0; i < 8; i++)
				alpha[i][(K +3)*stride +j] = -std::numeric_limits<R>::max();
		}

		// init beta values
		for (auto j = 0; j < mipp::nElmtsPerRegister<R>(); j++)
		{
			beta[0][(K +3)*stride +j] = (R)0;
			for (auto i = 1; i < 8; i++)
				beta[i][(K +3)*stride +j] = -std::numeric_limits<R>::max();
			for (auto i = 0; i < 8; i++)
				beta[i][j] = -std::numeric_limits<R>::max();
		}
	}
};

template <>
struct RSC_BCJR_inter_init <short>
{
	static void apply(mipp::vector<short> alpha[8], mipp::vector<short> beta[8], const int &K)
	{
		constexpr auto stride = mipp::nElmtsPerRegister<short>();

		// init alpha values
		for (auto j = 0; j < mipp::nElmtsPerRegister<short>(); j++)
		{
			alpha[0][j] = (short)0;
			for (auto i = 1; i < 8; i++)
				alpha[i][j] = -(1 << (sizeof(short) * 8 -2));
			for (auto i = 0; i < 8; i++)
				alpha[i][(K +3)*stride +j] = -(1 << (sizeof(short) * 8 -2));
		}

		// init beta values
		for (auto j = 0; j < mipp::nElmtsPerRegister<short>(); j++)
		{
			beta[0][(K +3)*stride +j] = (short)0;
			for (auto i = 1; i < 8; i++)
				beta[i][(K +3)*stride +j] = -(1 << (sizeof(short) * 8 -2));
			for (auto i = 0; i < 8; i++)
				beta[i][j] = -(1 << (sizeof(short) * 8 -2));
		}
	}
};

template <>
struct RSC_BCJR_inter_init <signed char>
{
	static void apply(mipp::vector<signed char> alpha[8], mipp::vector<signed char> beta[8], const int &K)
	{
		constexpr auto stride = mipp::nElmtsPerRegister<signed char>();

		// init alpha values
		for (auto j = 0; j < mipp::nElmtsPerRegister<signed char>(); j++)
		{
			alpha[0][j] = (signed char)0;
			for (auto i = 1; i < 8; i++)
				alpha[i][j] = -127;
			for (auto i = 0; i < 8; i++)
				alpha[i][(K +3)*stride +j] = -127;
		}

		// init beta values
		for (auto j = 0; j < mipp::nElmtsPerRegister<signed char>(); j++)
		{
			beta[0][(K +3)*stride +j] = (signed char)0;
			for (auto i = 1; i < 8; i++)
				beta[i][(K +3)*stride +j] = -127;
			for (auto i = 0; i < 8; i++)
				beta[i][j] = -127;
		}
	}
};

template <typename B, typename R>
Decoder_RSC_BCJR_inter<B,R>
::Decoder_RSC_BCJR_inter(const int &K, 
                         const std::vector<std::vector<int>> &trellis, 
                         const bool buffered_encoding,
                         const std::string name)
: Decoder_RSC_BCJR<B,R>(K, trellis, buffered_encoding, mipp::nElmtsPerRegister<R>(), name)
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
		{
			std::cerr << "(EE) This decoder does not support the input trellis... Exiting." << std::endl;
			exit(-1);
		}
		
	for (auto i = 0; i < 8; i++) alpha[i].resize((K +4) * mipp::nElmtsPerRegister<R>());
	for (auto i = 0; i < 8; i++) beta [i].resize((K +4) * mipp::nElmtsPerRegister<R>());
	for (auto i = 0; i < 2; i++) gamma[i].resize((K +3) * mipp::nElmtsPerRegister<R>());

	RSC_BCJR_inter_init<R>::apply(alpha, beta, K);
}

template <typename B, typename R>
Decoder_RSC_BCJR_inter<B,R>
::~Decoder_RSC_BCJR_inter()
{
}

template <typename B, typename R>
void Decoder_RSC_BCJR_inter<B,R>
::load(const mipp::vector<R>& Y_N)
{
	if (this->buffered_encoding && this->get_n_frames() > 1)
	{
		const auto tail = this->tail_length();

		constexpr auto n_frames = mipp::nElReg<R>();
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
void Decoder_RSC_BCJR_inter<B,R>
::store(mipp::vector<B>& V_K) const
{
	if (this->get_n_frames() > 1)
	{
		constexpr auto n_frames = mipp::nElReg<B>();

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
struct RSC_BCJR_inter_div_or_not
{
	static mipp::Reg<R> apply(mipp::Reg<R> r)
	{
		return mipp::div2(r);
	}
};

template <>
struct RSC_BCJR_inter_div_or_not <short>
{
	static mipp::Reg<short> apply(mipp::Reg<short> r)
	{
		// (WW) work only for max-log-MAP !!!
		return r;
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_inter_post
{
	static mipp::Reg<R> compute(const mipp::Reg<R> &r_post)
	{
		return r_post;
	}
};

template <>
struct RSC_BCJR_inter_post <short>
{
	static mipp::Reg<short> compute(const mipp::Reg<short> &r_post)
	{
		// (WW) work only for max-log-MAP !!!
		return mipp::div2(r_post);
	}
};

template <>
struct RSC_BCJR_inter_post <signed char>
{
	static mipp::Reg<signed char> compute(const mipp::Reg<signed char> &r_post)
	{
		return r_post.sat(-63, 63);
	}
};
