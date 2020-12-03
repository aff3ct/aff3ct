#ifndef BITWISE_DIFF_H__
#define BITWISE_DIFF_H__

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
/*
 * \brief compare as values of inputs, ie. compare if both are equals or not
 * when there is only one parameter the input is compared to zero
 * \param count_unknown_symbols is to consider as false a value that checks
 *        aff3ct::tools::is_unknown_symbol<T>(in) condition
 * \return 0 when equal else 1
 */
template<typename T, bool count_unknown_symbols>
struct Bitwise_diff
{
	static inline mipp::Reg<T> apply(const mipp::Reg<T>& in1, const mipp::Reg<T>& in2);

	static inline mipp::Reg<T> apply(const mipp::Reg<T>& in);

	static inline T apply(const T& in1, const T& in2);

	static inline T apply(const T& in);
};
}
}

#include "Tools/Perf/distance/Bitwise_diff.hxx"

#endif