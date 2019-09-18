#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <numeric>
#include <limits>
#include <cmath>

#include "Tools/Algo/Sort/LC_sorter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
LC_sorter<T>
::LC_sorter(const int max_elmts)
: max_elmts(max_elmts), vals(2 * max_elmts)
{
	tree_idx.resize(2 * max_elmts -1);
	std::iota(tree_idx.begin(), tree_idx.begin() + this->max_elmts, 0);
}

template <typename T>
void LC_sorter<T>
::partial_sort_destructive(T* values, std::vector<int> &pos, int n_elmts, int K)
{
	K       = (K       <= 0) ? (int)pos.size() : K;
	n_elmts = (n_elmts <= 0) ? max_elmts       : n_elmts;

	auto depth = (int)std::log2(n_elmts);

	if (n_elmts > max_elmts)
	{
		max_elmts = n_elmts;
		tree_idx.resize(2 * max_elmts -1);
		std::iota(tree_idx.begin(), tree_idx.begin() + this->max_elmts, 0);
		vals.resize(max_elmts);
	}

	LC_sorter<T>::_partial_sort_step1(values, pos, n_elmts, K, depth, max_elmts, tree_idx);
	if (K == 2)
		LC_sorter<T>::_partial_sort2_step2(values, pos, depth, max_elmts, tree_idx);
	else
		LC_sorter<T>::_partial_sort_step2(values, pos, K, depth, max_elmts, tree_idx);

}

template <typename T>
void LC_sorter<T>
::partial_sort(const T* values, std::vector<int> &pos, int n_elmts, int K)
{
	K       = (K       <= 0) ? (int)pos.size() : K;
	n_elmts = (n_elmts <= 0) ? max_elmts       : n_elmts;

	auto depth = (int)std::log2(n_elmts);

	if (n_elmts > max_elmts)
	{
		max_elmts = n_elmts;
		tree_idx.resize(2 * max_elmts -1);
		std::iota(tree_idx.begin(), tree_idx.begin() + this->max_elmts, 0);
		vals.resize(max_elmts);
	}

	LC_sorter<T>::_partial_sort_step1(values, pos, n_elmts, K, depth, max_elmts, tree_idx);
	if (K == 2)
		LC_sorter<T>::_partial_sort2_step2(values, pos, depth, max_elmts, tree_idx);
	else
	{
		// copy the "values" vector in "vals"
		for (auto i = 0; i < n_elmts; i++)
			vals[i] = values[i];

		LC_sorter<T>::_partial_sort_step2(vals.data(), pos, K, depth, max_elmts, tree_idx);
	}
}

template <typename T>
void LC_sorter<T>
::_partial_sort_step1(const T                *values,
                            std::vector<int> &pos,
                      const int               n_elmts,
                      const int               K,
                      const int               depth,
                      const int               max_elmts,
                            std::vector<int> &tree_idx)
{
	// sort all the tree (1)
	const auto n_2 = n_elmts >> 1;
	for (auto j = 0; j < n_2; j++)
	{
		const auto val0 = values[tree_idx[2*j +0]];
		const auto val1 = values[tree_idx[2*j +1]];

		tree_idx[max_elmts +j] = (val0 < val1) ? tree_idx[2*j +0] : tree_idx[2*j +1];
	}

	// sort all the tree (2)
	auto offset = max_elmts;
	for (auto n = 1 << (depth -1); n > 1; n >>= 1)
	{
		const auto n_2 = n >> 1;
		for (auto j = 0; j < n_2; j++)
		{
			const auto val0 = values[tree_idx[offset + 2*j +0]];
			const auto val1 = values[tree_idx[offset + 2*j +1]];

			tree_idx[offset + n +j] = (val0 < val1) ? tree_idx[offset + 2*j +0] : tree_idx[offset + 2*j +1];
		}

		offset += n;
	}

	// get the first min
	pos[0] = tree_idx[offset];
}

template <typename T>
void LC_sorter<T>
::_partial_sort_step2(      T                *values,
                            std::vector<int> &pos,
                      const int               K,
                      const int               depth,
                      const int               max_elmts,
                            std::vector<int> &tree_idx)
{
	for (auto k = 0; k < K -1; k++)
	{
		// replace the min val by +inf (+inf = max)
		values[pos[k]] = std::numeric_limits<T>::max();

		// compute only 1 branch (1)
		const auto j = pos[k] / 2;

		const auto val0 = values[tree_idx[2*j +0]];
		const auto val1 = values[tree_idx[2*j +1]];

		tree_idx[max_elmts +j] = (val0 < val1) ? tree_idx[2*j +0] : tree_idx[2*j +1];

		// compute only 1 branch (2)
		auto i = 1;
		auto offset = max_elmts;
		for (auto n = 1 << (depth -1); n > 1; n >>= 1)
		{
			const auto j = pos[k] / ((1 << i) * 2);

			const auto val0 = values[tree_idx[offset + 2*j +0]];
			const auto val1 = values[tree_idx[offset + 2*j +1]];

			tree_idx[offset + n +j] = (val0 < val1) ? tree_idx[offset + 2*j +0] : tree_idx[offset + 2*j +1];

			offset += n;
			i++;
		}

		// get an other min
		pos[k +1] = tree_idx[offset];
	}
}

template <typename T>
void LC_sorter<T>
::_partial_sort2_step2(const T                *values,
                             std::vector<int> &pos,
                       const int               depth,
                       const int               max_elmts,
                             std::vector<int> &tree_idx)
{
	// replace the min val by this opponent (rev depth of 1)
	tree_idx[max_elmts + (pos[0] / 2)] = (pos[0] % 2) ? pos[0] -1 : pos[0] +1;

	// update only one branch
	auto i = 2;
	auto offset = max_elmts;
	for (auto n = 1 << (depth -1); n > 1; n >>= 1)
	{
		const auto j = pos[0] / (1 << i);

		const auto val0 = values[tree_idx[offset + 2*j +0]];
		const auto val1 = values[tree_idx[offset + 2*j +1]];

		tree_idx[offset + n + j] = (val0 < val1) ? tree_idx[offset + 2*j +0] : tree_idx[offset + 2*j +1];

		offset += n;
		i++;
	}

	pos[1] = tree_idx[offset];
}
}
}
