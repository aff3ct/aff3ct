#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <numeric>
#include <limits>
#include <cmath>

#include "Tools/Algo/Sort/LC_sorter.hpp"
#include "Tools/Algo/Sort/LC_sorter_simd.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
LC_sorter_simd<T>
::LC_sorter_simd(const int max_elmts)
: max_elmts(max_elmts), vals(2 * max_elmts)
{
	tree_idx.resize(2 * max_elmts -1);
	std::iota(tree_idx.begin(), tree_idx.begin() + this->max_elmts, 0);
}

template <typename T>
void LC_sorter_simd<T>
::partial_sort_abs(const T* values, std::vector<int> &pos, int n_elmts, int K)
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

	// copy the "values" vector in "vals"
	for (auto i = 0; i < n_elmts; i++)
		vals[i] = std::abs(values[i]);

	if (n_elmts >= 2 * mipp::nElReg<T>())
	{
		_partial_sort_step1(vals.data(), pos, n_elmts, K, depth);

		if (K > 1)
			_partial_sort_step2(vals.data(), pos, K, depth);
	}
	else
	{
		LC_sorter<T>::_partial_sort_step1(vals.data(), pos, n_elmts, K, depth, max_elmts, tree_idx);
		if (K == 2)
			LC_sorter<T>::_partial_sort2_step2(vals.data(), pos, depth, max_elmts, tree_idx);
		else
			LC_sorter<T>::_partial_sort_step2(vals.data(), pos, K, depth, max_elmts, tree_idx);
	}
}

template <typename T>
void LC_sorter_simd<T>
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

	if (n_elmts >= 2 * mipp::nElReg<T>())
	{
		// copy the "values" vector in "vals"
		for (auto i = 0; i < n_elmts; i++)
			vals[i] = values[i];

		_partial_sort_step1(vals.data(), pos, n_elmts, K, depth);

		if (K > 1)
			_partial_sort_step2(vals.data(), pos, K, depth);
	}
	else
	{
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
}

template <typename T>
void LC_sorter_simd<T>
::_partial_sort_step1(T* values, std::vector<int> &pos, const int n_elmts, const int K, const int depth)
{
	// sort all the tree (1)
	const auto n_elmts_2 = n_elmts >> 1;
	for (auto j = 0; j < n_elmts_2; j += mipp::nElReg<T>())
	{
		const auto val0 = mipp::Reg<T  >(&values  [2*j + 0*mipp::nElReg<T>()]); // load
		const auto val1 = mipp::Reg<T  >(&values  [2*j + 1*mipp::nElReg<T>()]); // load
		const auto idx0 = mipp::Reg<int>(&tree_idx[2*j + 0*mipp::nElReg<T>()]); // load
		const auto idx1 = mipp::Reg<int>(&tree_idx[2*j + 1*mipp::nElReg<T>()]); // load

		const auto min  = mipp::min(val0, val1);
		const auto idx  = mipp::blend(idx0, idx1, val0 < val1);

		min.store(&values  [max_elmts +j]); // store
		idx.store(&tree_idx[max_elmts +j]); // store
	}

	// sort all the tree (2)
	auto offset = max_elmts;
	for (auto n = n_elmts_2; n > mipp::nElReg<T>(); n >>= 1)
	{
		const auto n_2 = (n >> 1);
		for (auto j = 0; j < n_2; j += mipp::nElReg<T>())
		{
			const auto val0 = mipp::Reg<T  >(&values  [offset + 2*j + 0*mipp::nElReg<T>()]); // load
			const auto val1 = mipp::Reg<T  >(&values  [offset + 2*j + 1*mipp::nElReg<T>()]); // load
			const auto idx0 = mipp::Reg<int>(&tree_idx[offset + 2*j + 0*mipp::nElReg<T>()]); // load
			const auto idx1 = mipp::Reg<int>(&tree_idx[offset + 2*j + 1*mipp::nElReg<T>()]); // load

			const auto min   = mipp::min(val0, val1);
			const auto idx   = mipp::blend(idx0, idx1, val0 < val1);

			min.store(&values  [offset + n +j]); // store
			idx.store(&tree_idx[offset + n +j]); // store
		}

		offset += n;
	}

	// sequential part (searching the min pos)
	auto min_pos = 0;
	auto min = std::numeric_limits<T>::max();

	for (auto i = 0; i < mipp::nElReg<T>(); i++)
	{
		min_pos = values[tree_idx[offset +i]] < min ? tree_idx[offset +i] : min_pos;
		min = std::min(values[tree_idx[offset +i]], min);
	}

	pos[0] = min_pos;
}

template <typename T>
void LC_sorter_simd<T>
::_partial_sort_step2(T* values, std::vector<int> &pos, const int K, const int depth)
{
	for (auto k = 0; k < (int)K -1; k++)
	{
		// replace the min val by +inf (+inf = max)
		values[pos[k]] = std::numeric_limits<T>::max();

		// compute only 1 branch (1)
		{
			const auto node = pos[k];
			const auto j    = (node / (mipp::nElReg<T>() * 2)) * mipp::nElReg<T>();

			const auto val0 = mipp::Reg<T  >(&values  [2*j + 0*mipp::nElReg<T>()]); // load
			const auto val1 = mipp::Reg<T  >(&values  [2*j + 1*mipp::nElReg<T>()]); // load
			const auto idx0 = mipp::Reg<int>(&tree_idx[2*j + 0*mipp::nElReg<T>()]); // load
			const auto idx1 = mipp::Reg<int>(&tree_idx[2*j + 1*mipp::nElReg<T>()]); // load

			const auto min   = mipp::min(val0, val1);
			const auto idx   = mipp::blend(idx0, idx1, val0 < val1);

			min.store(&values  [max_elmts +j]); // store
			idx.store(&tree_idx[max_elmts +j]); // store
		}

		// compute only 1 branch (2)
		auto i      = 1;
		auto offset = max_elmts;
		for (auto n = 1 << (depth -1); n > mipp::nElReg<T>(); n >>= 1)
		{
			const auto node = pos[k] / (1 << i);
			const auto j    = (node / (mipp::nElReg<T>() * 2)) * mipp::nElReg<T>();

			const auto val0 = mipp::Reg<T  >(&values  [offset + 2*j + 0*mipp::nElReg<T>()]); // load
			const auto val1 = mipp::Reg<T  >(&values  [offset + 2*j + 1*mipp::nElReg<T>()]); // load
			const auto idx0 = mipp::Reg<int>(&tree_idx[offset + 2*j + 0*mipp::nElReg<T>()]); // load
			const auto idx1 = mipp::Reg<int>(&tree_idx[offset + 2*j + 1*mipp::nElReg<T>()]); // load

			const auto min   = mipp::min(val0, val1);
			const auto idx   = mipp::blend(idx0, idx1, val0 < val1);

			min.store(&values  [offset + n +j]); // store
			idx.store(&tree_idx[offset + n +j]); // store

			offset += n;
			i++;
		}

		// sequential part (searching the min pos)
		auto min_pos = 0;
		auto min = std::numeric_limits<T>::max();

		for (auto i = 0; i < mipp::nElReg<T>(); i++)
		{
			min_pos = values[tree_idx[offset +i]] < min ? tree_idx[offset +i] : min_pos;
			min = std::min(values[tree_idx[offset +i]], min);
		}

		pos[k +1] = min_pos;
	}
}
}
}
