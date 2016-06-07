#include "Decoder_RSC_BCJR_inter.hpp"

#include <limits>

#include "../../../../Tools/MIPP/mipp.h"

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
::Decoder_RSC_BCJR_inter(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, buffered_encoding, mipp::nElmtsPerRegister<R>())
{
	for (auto i = 0; i < 8; i++) alpha[i].resize((this->K +4) * mipp::nElmtsPerRegister<R>());
	for (auto i = 0; i < 8; i++) beta [i].resize((this->K +4) * mipp::nElmtsPerRegister<R>());
	for (auto i = 0; i < 2; i++) gamma[i].resize((this->K +3) * mipp::nElmtsPerRegister<R>());

	RSC_BCJR_inter_init<R>::apply(alpha, beta, this->K);
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
	static mipp::reg apply(mipp::reg r)
	{
		return r;
	}
};

template <>
struct RSC_BCJR_inter_div_or_not <signed char>
{
	static mipp::reg apply(mipp::reg r)
	{
		return mipp::div2<signed char>(r);
	}
};

// ====================================================================================================== post division
template <typename R>
struct RSC_BCJR_inter_post
{
	static mipp::reg compute(const mipp::reg &r_post)
	{
		return mipp::div2<R>(r_post);
	}
};

template <>
struct RSC_BCJR_inter_post <signed char>
{
	static mipp::reg compute(const mipp::reg &r_post)
	{
		return mipp::sat<signed char>(r_post, -63, 63);
	}
};