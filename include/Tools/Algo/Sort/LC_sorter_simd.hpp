/*!
 * \file
 * \brief Class tools::LC_sorter_simd.
 */
#ifndef LC_SORTER_SIMD_HPP
#define LC_SORTER_SIMD_HPP

#include <vector>
#include <mipp.h>

namespace aff3ct
{
namespace tools
{
template <typename T>
class LC_sorter_simd
{
private:
	int               max_elmts;
	mipp::vector<int> tree_idx;
	mipp::vector<T>   vals;

public:
	explicit LC_sorter_simd(const int max_elmts);

	void partial_sort_abs(const T* values, std::vector<int> &pos, int n_elmts = -1, int K = -1);

	void partial_sort(const T* values, std::vector<int> &pos, int n_elmts = -1, int K = -1);

private:
	inline void _partial_sort_step1(T* values, std::vector<int> &pos, const int n_elmts, const int K, const int depth);

	inline void _partial_sort_step2(T* values, std::vector<int> &pos, const int K, const int depth);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Sort/LC_sorter_simd.hxx"
#endif

#endif /* LC_SORTER_SIMD_HPP */
