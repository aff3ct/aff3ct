#ifndef BOOLEAN_DIFF_H__
#define BOOLEAN_DIFF_H__

#include <mipp.h>

namespace aff3ct
{
namespace tools
{

/*
 * \brief compare as boolean the inputs, ie. compare if both equals 0 or is 1 (!= 0)
 * when there is only one parameter the input is compared to zero
 * \param T if real, compare signs
 * \param count_unknown_symbols is to consider as false a value that checks
 *        aff3ct::tools::is_unknown_symbol<T>(in) condition
 * \return 0 when equal else 1
 */
template<typename T, bool count_unknown_symbols>
struct Boolean_diff
{
	static inline mipp::Reg<T> apply(const mipp::Reg<T>& in1, const mipp::Reg<T>& in2);

	static inline mipp::Reg<T> apply(const mipp::Reg<T>& in);

	static inline T apply(const T& in1, const T& in2);

	static inline T apply(const T& in);
};

}
}

#include "Tools/Perf/distance/Boolean_diff.hxx"

#endif