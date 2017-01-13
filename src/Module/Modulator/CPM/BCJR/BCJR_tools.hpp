#ifndef BCJR_TOOLS_HPP_
#define BCJR_TOOLS_HPP_

#include "Tools/Math/utils.h"

template<typename Q>
inline Q negative_inf(){return -std::numeric_limits<Q>::max(); }

template<>
inline short negative_inf<short>(){return -(1 << (sizeof(short) * 8 -2)); }

template<>
inline signed char negative_inf<signed char>(){return -63; }

template<typename Q>
inline Q positive_inf(){return std::numeric_limits<Q>::max(); }

template<>
inline short positive_inf<short>(){return (1 << (sizeof(short) * 8 -2)); }

template<>
inline signed char positive_inf<signed char>(){return 63; }



template <typename Q, proto_max<Q> MAX>
inline void BCJR_normalize(Q *metrics, const int &i, const int &n_states)
{
	// normalization
	auto norm_val = negative_inf<Q>();
	for (auto j = 0; j < n_states; j++)
		norm_val = MAX(norm_val, metrics[j]);

	for (auto j = 0; j < n_states; j++)
		metrics[j] -= norm_val;
}

template <signed char, proto_max<signed char> MAX>
inline void BCJR_normalize(signed char *metrics, const int &i, const int &n_states)
{
	// normalization
	auto norm_val = negative_inf<signed char>();
	for (auto j = 0; j < n_states; j++)
		norm_val = MAX(norm_val, metrics[j]);

	for (auto j = 0; j < n_states; j++)
		metrics[j] = saturate<signed char>(metrics[j] - norm_val, -63, +63);
}

#endif /* BCJR_TOOLS_HPP_ */
