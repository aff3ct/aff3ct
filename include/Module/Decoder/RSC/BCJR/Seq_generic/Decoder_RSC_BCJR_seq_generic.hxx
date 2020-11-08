#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <limits>

#include "Tools/Math/utils.h"
#include "Module/Decoder/RSC/BCJR/Seq_generic/Decoder_RSC_BCJR_seq_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename R>
struct RSC_BCJR_seq_generic_init
{
	static void apply(mipp::vector<mipp::vector<R>> &alpha,
	                  mipp::vector<mipp::vector<R>> &beta,
	                  const int &K, const int &n_states, const int &n_ff)
	{
		// init alpha values
		alpha[0][0] = (R)0;
		for (auto i = 1; i < n_states; i++)
			alpha[i][0] = -std::numeric_limits<R>::max();
		for (auto i = 0; i < n_states; i++)
			alpha[i][K +n_ff] = -std::numeric_limits<R>::max();

		// init beta values
		beta[0][K +n_ff] = (R)0;
		for (auto i = 1; i < n_states; i++)
			beta[i][K +n_ff] = -std::numeric_limits<R>::max();
		for (auto i = 0; i < n_states; i++)
			beta[i][0] = -std::numeric_limits<R>::max();
	}
};

template <>
struct RSC_BCJR_seq_generic_init <short>
{
	static void apply(mipp::vector<mipp::vector<short>> &alpha,
	                  mipp::vector<mipp::vector<short>> &beta,
	                  const int &K, const int &n_states, const int &n_ff)
	{
		// init alpha values
		alpha[0][0] = (short)0;
		for (auto i = 1; i < n_states; i++)
			alpha[i][0] = -(1 << (sizeof(short) * 8 -2));
		for (auto i = 0; i < n_states; i++)
			alpha[i][K +n_ff] = -(1 << (sizeof(short) * 8 -2));

		// init beta values
		beta[0][K +n_ff] = (short)0;
		for (auto i = 1; i < n_states; i++)
			beta[i][K +n_ff] = -(1 << (sizeof(short) * 8 -2));
		for (auto i = 0; i < n_states; i++)
			beta[i][0] = -(1 << (sizeof(short) * 8 -2));
	}
};

template <>
struct RSC_BCJR_seq_generic_init <signed char>
{
	static void apply(mipp::vector<mipp::vector<signed char>> &alpha,
	                  mipp::vector<mipp::vector<signed char>> &beta,
	                  const int &K, const int &n_states, const int &n_ff)
	{
		// init alpha values
		alpha[0][0] = (signed char)0;
		for (auto i = 1; i < n_states; i++)
			alpha[i][0] = -63;
		for (auto i = 0; i < n_states; i++)
			alpha[i][K +n_ff] = -63;

		// init beta values
		beta[0][K +n_ff] = (signed char)0;
		for (auto i = 1; i < n_states; i++)
			beta[i][K +n_ff] = -63;
		for (auto i = 0; i < n_states; i++)
			beta[i][0] = -63;
	}
};

template <typename B, typename R>
Decoder_RSC_BCJR_seq_generic<B,R>
::Decoder_RSC_BCJR_seq_generic(const int &K,
                               const std::vector<std::vector<int>> &trellis,
                               const bool buffered_encoding)
: Decoder_RSC_BCJR<B,R>(K, trellis, buffered_encoding),
  alpha(this->n_states), beta(this->n_states), gamma(2)
{
	const std::string name = "Decoder_RSC_BCJR_seq_generic";
	this->set_name(name);

	for (unsigned i = 0; i < alpha.size(); i++) alpha[i].resize(K + this->n_ff +1);
	for (unsigned i = 0; i < beta .size(); i++) beta [i].resize(K + this->n_ff +1);
	for (unsigned i = 0; i < gamma.size(); i++) gamma[i].resize(K + this->n_ff +0);

	RSC_BCJR_seq_generic_init<R>::apply(alpha, beta, K, this->n_states, this->n_ff);
}

template <typename B, typename R>
Decoder_RSC_BCJR_seq_generic<B,R>* Decoder_RSC_BCJR_seq_generic<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

// =================================================================================================== sys/par division
template <typename R>
struct RSC_BCJR_seq_generic_div_or_not
{
	static R apply(R m)
	{
		return tools::div2<R>(m);
	}
};

template <>
struct RSC_BCJR_seq_generic_div_or_not <short>
{
	static short apply(short m)
	{
		// (WW) work only for max-log-MAP !!!
		return m;
	}
};

// ====================================================================================================== post division
template <typename R, typename RD>
struct RSC_BCJR_seq_generic_post
{
	static R compute(const RD &post)
	{
		return post;
	}
};

template <typename RD>
struct RSC_BCJR_seq_generic_post <short, RD>
{
	static short compute(const RD &post)
	{
		// (WW) work only for max-log-MAP !!!
		return tools::div2<RD>(post);
	}
};

template <typename RD>
struct RSC_BCJR_seq_generic_post <signed char, RD>
{
	static signed char compute(const RD &post)
	{
		return (signed char)tools::saturate<RD>(post, -63, 63);
	}
};

// ====================================================================================================== normalization
template <typename R>
struct RSC_BCJR_seq_generic_normalize
{
	static void apply(mipp::vector<mipp::vector<R>> &metrics, const int &i, const int &n_states)
	{
		// no need to do something
	}

	static void apply(R *metrics, const int &i, const int &n_states)
	{
		// no need to do something
	}
};

template <>
struct RSC_BCJR_seq_generic_normalize <short>
{
	static void apply(mipp::vector<mipp::vector<short>> &metrics, const int &i, const int &n_states)
	{
		// normalization
		if (i % 8 == 0)
		{
			auto norm_val = metrics[0][i];
			for (auto j = 0; j < n_states; j++)
				metrics[j][i] -= norm_val;
		}
	}

	static void apply(short *metrics, const int &i, const int &n_states)
	{
		// normalization
		if (i % 8 == 0)
		{
			auto norm_val = metrics[0];
			for (auto j = 0; j < n_states; j++)
				metrics[j] -= norm_val;
		}
	}
};

template <>
struct RSC_BCJR_seq_generic_normalize <signed char>
{
	static void apply(mipp::vector<mipp::vector<signed char>> &metrics, const int &i, const int &n_states)
	{
		// normalization & saturation
		auto norm_val = metrics[0][i];
		for (auto j = 0; j < n_states; j++)
			metrics[j][i] = tools::saturate<signed char>(metrics[j][i] - norm_val, -63, +63);
	}

	static void apply(signed char *metrics, const int &i, const int &n_states)
	{
		// normalization & saturation
		auto norm_val = metrics[0];
		for (auto j = 0; j < n_states; j++)
			metrics[j] = tools::saturate<signed char>(metrics[j] - norm_val, -63, +63);
	}
};
}
}
