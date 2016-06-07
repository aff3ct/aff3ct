#include <limits>

#include "Decoder_RSC_BCJR_seq.hpp"

template <typename R>
struct RSC_BCJR_seq_init
{
	static void apply(mipp::vector<R> alpha[8], mipp::vector<R> beta[8], const int &K)
	{
		// init alpha values
		alpha[0][0] = (R)0;
		for (auto i = 1; i < 8; i++)
			alpha[i][0] = -std::numeric_limits<R>::max();
		for (auto i = 0; i < 8; i++)
			alpha[i][K +3] = -std::numeric_limits<R>::max();

		// init beta values
		beta[0][K +3] = (R)0;
		for (auto i = 1; i < 8; i++)
			beta[i][K +3] = -std::numeric_limits<R>::max();
		for (auto i = 0; i < 8; i++)
			beta[i][0] = -std::numeric_limits<R>::max();
	}
};

template <>
struct RSC_BCJR_seq_init <short>
{
	static void apply(mipp::vector<short> alpha[8], mipp::vector<short> beta[8], const int &K)
	{
		// init alpha values
		alpha[0][0] = (short)0;
		for (auto i = 1; i < 8; i++)
			alpha[i][0] = -(1 << (sizeof(short) * 8 -2));
		for (auto i = 0; i < 8; i++)
			alpha[i][K +3] = -(1 << (sizeof(short) * 8 -2));

		// init beta values
		beta[0][K +3] = (short)0;
		for (auto i = 1; i < 8; i++)
			beta[i][K +3] = -(1 << (sizeof(short) * 8 -2));
		for (auto i = 0; i < 8; i++)
			beta[i][0] = -(1 << (sizeof(short) * 8 -2));
	}
};

template <>
struct RSC_BCJR_seq_init <signed char>
{
	static void apply(mipp::vector<signed char> alpha[8], mipp::vector<signed char> beta[8], const int &K)
	{
		// init alpha values
		alpha[0][0] = (signed char)0;
		for (auto i = 1; i < 8; i++)
			alpha[i][0] = -63;
		for (auto i = 0; i < 8; i++)
			alpha[i][K +3] = -63;

		// init beta values
		beta[0][K +3] = (signed char)0;
		for (auto i = 1; i < 8; i++)
			beta[i][K +3] = -63;
		for (auto i = 0; i < 8; i++)
			beta[i][0] = -63;
	}
};

template <typename B, typename R>
Decoder_RSC_BCJR_seq<B,R>
::Decoder_RSC_BCJR_seq(const int &K, const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, buffered_encoding)
{
	for (auto i = 0; i < 8; i++) alpha[i].resize(K +4);
	for (auto i = 0; i < 8; i++) beta [i].resize(K +4);
	for (auto i = 0; i < 2; i++) gamma[i].resize(K +3);

	RSC_BCJR_seq_init<R>::apply(alpha, beta, K);
}

template <typename B, typename R>
Decoder_RSC_BCJR_seq<B,R>
::~Decoder_RSC_BCJR_seq()
{
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_seq_div_or_not
{
	static R apply(R m)
	{
		return m;
	}
};

template <>
struct RSC_BCJR_seq_div_or_not <signed char>
{
	static signed char apply(signed char m)
	{
		return div2<signed char>(m);
	}
};

// ====================================================================================================== post division
template <typename R, typename RD>
struct RSC_BCJR_seq_post
{
	static R compute(const RD &post)
	{
		return div2<R>(post);
	}
};

template <typename RD>
struct RSC_BCJR_seq_post <signed char, RD>
{
	static signed char compute(const RD &post)
	{
		return (signed char)saturate<RD>(post, -63, 63);
	}
};

// ====================================================================================================== normalization
template <typename R>
struct RSC_BCJR_seq_normalize
{
	static void apply(mipp::vector<R> metrics[8], const int &i)
	{
		// no need to do something
	}

	static void apply(R metrics[8][mipp::nElReg<R>()], const int &i)
	{
		// no need to do something
	}

	static void apply(R metrics[8], const int &i)
	{
		// no need to do something
	}
};

template <>
struct RSC_BCJR_seq_normalize <short>
{
	static void apply(mipp::vector<short> metrics[8], const int &i)
	{
		// normalization
		if (i % 8 == 0)
		{
			auto norm_val = metrics[0][i];
			for (auto j = 0; j < 8; j++)
				metrics[j][i] -= norm_val;
		}
	}

	static void apply(short metrics[8][mipp::nElReg<short>()], const int &i)
	{
		// normalization
		if (i % 8 == 0)
		{
			auto norm_val = metrics[0][i];
			for (auto j = 0; j < 8; j++)
				metrics[j][i] -= norm_val;
		}
	}


	static void apply(short metrics[8], const int &i)
	{
		// normalization
		if (i % 8 == 0)
		{
			auto norm_val = metrics[0];
			for (auto j = 0; j < 8; j++)
				metrics[j] -= norm_val;
		}
	}
};

template <>
struct RSC_BCJR_seq_normalize <signed char>
{
	static void apply(mipp::vector<signed char> metrics[8], const int &i)
	{
		// normalization & saturation
		auto norm_val = metrics[0][i];
		for (auto j = 0; j < 8; j++)
			metrics[j][i] = saturate<signed char>(metrics[j][i] - norm_val, -63, +63);
	}

	static void apply(signed char metrics[8][mipp::nElReg<signed char>()], const int &i)
	{
		// normalization & saturation
		auto norm_val = metrics[0][i];
		for (auto j = 0; j < 8; j++)
			metrics[j][i] = saturate<signed char>(metrics[j][i] - norm_val, -63, +63);
	}

	static void apply(signed char metrics[8], const int &i)
	{
		// normalization & saturation
		auto norm_val = metrics[0];
		for (auto j = 0; j < 8; j++)
			metrics[j] = saturate<signed char>(metrics[j] - norm_val, -63, +63);
	}
};