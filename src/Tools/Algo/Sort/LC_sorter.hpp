#ifndef LC_SORTER_HPP
#define LC_SORTER_HPP

#include <cmath>
#include <vector>
#include <algorithm>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/utils.h"

template <typename T>
class LC_sorter
{
private:
	const int         size;
	const int         depth;
	mipp::vector<int> tree_idx;
	mipp::vector<T>   vals;

public:
	LC_sorter(const int size) : size(size), depth(std::log2(size)), vals(2 * size)
	{
		assert(is_power_of_2(size));

		int depth = std::log2(size);
		auto n_nodes = 0;
		for (auto i = depth; i >= 0; i--)
			n_nodes += std::exp2(i);
		tree_idx.resize(n_nodes);

		std::iota(tree_idx.begin(), tree_idx.begin() + this->size, 0);
	}

	int get_size()
	{
		return this->size;
	}

	void sort(const T* values, std::vector<int> &pos, const int p_sort = -1)
	{
		const auto K = (p_sort <= 0) ? (int)pos.size() : p_sort;

		if (K <= 2)
			sort2(values, pos, K);
		else // pos.size() > 2
		{
			// copy the "values" vector in "vals"
			for (auto i = 0; i < size; i++)
				vals[i] = values[i];

			if (K >= 1)
			{
				// sort all the tree
				auto offset = 0;
				for (auto n_elmts = 1 << depth; n_elmts > 1; n_elmts >>= 1)
				{
					const auto n_elm_2 = (n_elmts >> 1);
					for (auto j = 0; j < n_elm_2; j++)
					{
						const auto val0 = vals[tree_idx[offset + 2*j +0]];
						const auto val1 = vals[tree_idx[offset + 2*j +1]];

						tree_idx[offset + n_elmts +j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
						                                                tree_idx[offset + 2*j +1];
					}

					offset += n_elmts;
				}

				pos[0] = tree_idx[tree_idx.size() -1];
			}

			for (auto k = 0; k < K -1; k++)
			{
				// replace the min val by +inf (+inf = max)
				vals[pos[k]] = std::numeric_limits<T>::max();

				// compute only 1 branch
				auto i = 0;
				auto offset = 0;
				for (auto n_elmts = 1 << depth; n_elmts > 1; n_elmts >>= 1)
				{
					const auto j = pos[k] / ((1 << i) * 2);

					const auto val0 = vals[tree_idx[offset + 2*j +0]];
					const auto val1 = vals[tree_idx[offset + 2*j +1]];

					tree_idx[offset + n_elmts +j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
					                                                tree_idx[offset + 2*j +1];

					offset += n_elmts;
					i++;
				}

				// get an other min
				pos[k +1] = tree_idx[tree_idx.size() -1];
			}
		}
	}

	void sort_simd(const T* values, std::vector<int> &pos, const int p_sort = -1)
	{
		const auto K = (p_sort <= 0) ? (int)pos.size() : p_sort;

		// copy the "values" vector in "vals"
		for (auto i = 0; i < size; i++)
			vals[i] = std::abs(values[i]);

		// sort all the tree
		if (K >= 1)
		{
			auto offset  = 0;

			// vectorized part
			for (auto n_elmts = 1 << depth; n_elmts > mipp::nElReg<T>(); n_elmts >>= 1)
			{
				const auto n_elm_2 = (n_elmts >> 1);
				for (auto j = 0; j < n_elm_2; j += mipp::nElReg<T>())
				{
					const auto val0 = mipp::Reg<T  >(&vals    [offset + 2*j + 0*mipp::nElReg<T>()]); // load
					const auto val1 = mipp::Reg<T  >(&vals    [offset + 2*j + 1*mipp::nElReg<T>()]); // load
					const auto idx0 = mipp::Reg<int>(&tree_idx[offset + 2*j + 0*mipp::nElReg<T>()]); // load
					const auto idx1 = mipp::Reg<int>(&tree_idx[offset + 2*j + 1*mipp::nElReg<T>()]); // load

					const auto mask1 = val0 < val1;
					const auto mask2 = mask1.template cvt<int>();
					const auto min   = mipp::min  (val0, val1);
					const auto idx   = mipp::blend(idx1, idx0, mask2);

					min.store(&vals    [offset + n_elmts +j]); // store
					idx.store(&tree_idx[offset + n_elmts +j]); // store
				}

				offset += n_elmts;
			}

			// sequential part (or INTRA part)
			const auto start = size < mipp::nElReg<T>() ? size : mipp::nElReg<T>();
			for (auto n_elmts = start; n_elmts > 1; n_elmts >>= 1)
			{
				const auto n_elm_2 = (n_elmts >> 1);
				for (auto j = 0; j < n_elm_2; j++)
				{
					const auto val0 = vals[tree_idx[offset + 2*j +0]];
					const auto val1 = vals[tree_idx[offset + 2*j +1]];

					tree_idx[offset + n_elmts +j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
					                                                tree_idx[offset + 2*j +1];
				}

				offset += n_elmts;
			}

			pos[0] = tree_idx[tree_idx.size() -1];
		}

		if (K >= 2)
		{
			for (auto k = 0; k < (int)K -1; k++)
			{
				// replace the min val by +inf (+inf = max)
				vals[pos[k]] = std::numeric_limits<T>::max();

				// compute only 1 branch
				auto i      = 0;
				auto offset = 0;
				for (auto n_elmts = 1 << depth; n_elmts > mipp::nElReg<T>(); n_elmts >>= 1)
				{

					const auto node = pos[k] / (1 << i);
					const auto j    = (node / (mipp::nElReg<T>() * 2)) * mipp::nElReg<T>();

					const auto val0 = mipp::Reg<T  >(&vals    [offset + 2*j + 0*mipp::nElReg<T>()]); // load
					const auto val1 = mipp::Reg<T  >(&vals    [offset + 2*j + 1*mipp::nElReg<T>()]); // load
					const auto idx0 = mipp::Reg<int>(&tree_idx[offset + 2*j + 0*mipp::nElReg<T>()]); // load
					const auto idx1 = mipp::Reg<int>(&tree_idx[offset + 2*j + 1*mipp::nElReg<T>()]); // load

					const auto mask1 = val0 < val1;
					const auto mask2 = mask1.template cvt<int>();
					const auto min   = mipp::min  (val0, val1);
					const auto idx   = mipp::blend(idx1, idx0, mask2);

					min.store(&vals    [offset + n_elmts +j]); // store
					idx.store(&tree_idx[offset + n_elmts +j]); // store

					offset += n_elmts;
					i++;
				}

				const auto start = size < mipp::nElReg<T>() ? size : mipp::nElReg<T>();
				for (auto n_elmts = start; n_elmts > 1; n_elmts >>= 1)
				{
					const auto n_elm_2 = (n_elmts >> 1);
					for (auto j = 0; j < n_elm_2; j++)
					{
						const auto val0 = vals[tree_idx[offset + 2*j +0]];
						const auto val1 = vals[tree_idx[offset + 2*j +1]];

						tree_idx[offset + n_elmts +j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
						                                                tree_idx[offset + 2*j +1];
					}

					offset += n_elmts;
				}

				// get an other min
				pos[k +1] = tree_idx[tree_idx.size() -1];
			}
		}
	}

private:
	void sort2(const T* values, std::vector<int> &pos, const int K)
	{
		// sort all the tree
		auto offset = 0;
		for (auto n_elmts = 1 << depth; n_elmts > 1; n_elmts >>= 1)
		{
			const auto n_elm_2 = (n_elmts >> 1);
			for (auto j = 0; j < n_elm_2; j++)
			{
				const auto val0 = values[tree_idx[offset + 2*j +0]];
				const auto val1 = values[tree_idx[offset + 2*j +1]];

				tree_idx[offset + n_elmts +j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
				                                                tree_idx[offset + 2*j +1];
			}

			offset += n_elmts;
		}

		pos[0] = tree_idx[tree_idx.size() -1];

		if (K == 2)
		{
			// replace the min val by this opponent (rev depth of 1)
			tree_idx[(1 << depth) + (pos[0] / 2)] = (pos[0] % 2) ? pos[0] -1:
			                                                       pos[0] +1;

			// update only one branch
			auto i = 2;
			auto offset = (1 << depth);
			for (auto n_elmts = 1 << (depth -1); n_elmts > 1; n_elmts >>= 1)
			{
				const auto j = pos[0] / (1 << i);

				const auto val0 = values[tree_idx[offset + 2*j +0]];
				const auto val1 = values[tree_idx[offset + 2*j +1]];

				tree_idx[offset + n_elmts + j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
				                                                 tree_idx[offset + 2*j +1];

				offset += n_elmts;
				i++;
			}

			// gets the second min
			pos[1] = tree_idx[tree_idx.size() -1];
		}
	}
};

#endif /* LC_SORTER_HPP */
