#ifndef LC_SORTER_HPP
#define LC_SORTER_HPP

#include <vector>

namespace aff3ct
{
namespace tools
{

template <typename T>
class LC_sorter_simd;

template <typename T>
class LC_sorter
{
	friend LC_sorter_simd<T>;

private:
	int              max_elmts;
	std::vector<int> tree_idx;
	std::vector<T>   vals;

public:
	explicit LC_sorter(const int max_elmts);

	inline void partial_sort_destructive(T* values, std::vector<int> &pos, int n_elmts = -1, int K = -1);

	inline void partial_sort(const T* values, std::vector<int> &pos, int n_elmts = -1, int K = -1);

protected:
	static void _partial_sort_step1(const T                *values,
	                                      std::vector<int> &pos,
	                                const int               n_elmts,
	                                const int               K,
	                                const int               depth,
	                                const int               max_elmts,
	                                      std::vector<int> &tree_idx);

	static void _partial_sort_step2(      T                *values,
	                                      std::vector<int> &pos,
	                                const int               K,
	                                const int               depth,
	                                const int               max_elmts,
	                                      std::vector<int> &tree_idx);

	static void _partial_sort2_step2(const T                *values,
	                                       std::vector<int> &pos,
	                                 const int               depth,
	                                 const int               max_elmts,
	                                       std::vector<int> &tree_idx);
};
}
}

#include "Tools/Algo/Sort/LC_sorter.hxx"

#endif /* LC_SORTER_HPP */
