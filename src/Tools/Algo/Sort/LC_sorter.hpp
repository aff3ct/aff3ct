#ifndef LC_SORTER_HPP
#define LC_SORTER_HPP

#include <cmath>
#include <vector>
#include <algorithm>

#include "Tools/Math/utils.h"

class LC_sorter
{
private:
	const int        size;
	const int        depth;
	std::vector<int> tree_idx;

public:
	LC_sorter(const int size) : size(size), depth(std::log2(size))
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

	template <typename T>
	std::pair<int,int> sort(T* values)
	{
		std::pair<int,int> result(0,0);

		// on trie tout l'arbre (figure (a) sur le dessin que je t'ai envoyé)
		auto offset = 0;
		for (auto i = depth; i > 0; i--)
		{
			const auto n_elmts = (1 << i);
			const auto n_elm_2 = (n_elmts >> 1);

			for (auto j = 0; j < n_elm_2; j++)
			{
				const auto val0 = std::abs(values[tree_idx[offset + 2*j +0]]);
				const auto val1 = std::abs(values[tree_idx[offset + 2*j +1]]);

				tree_idx[offset + n_elmts +j] = (val0 < val1) ? tree_idx[offset + 2*j +0]:
				                                                tree_idx[offset + 2*j +1];
			}

			offset += n_elmts;
		}

		result.first = tree_idx[tree_idx.size() -1];

		// on remplace la valeur min (ou max) par son concurrent à reverse depth de 1
		tree_idx[(1 << depth) + (result.first / 2)] = (result.first % 2) ? result.first -1:
		                                                                   result.first +1;

		// on parcourt seulement la branche concernée par se changement
		offset = (1 << depth);
		for (auto i = depth -1; i > 0; i--)
		{
			const auto n_elmts = (1 << i);
			const auto node    = result.first / (1 << (depth - i + 1));

			const auto val0 = std::abs(values[tree_idx[offset + 2*node +0]]);
			const auto val1 = std::abs(values[tree_idx[offset + 2*node +1]]);

			tree_idx[offset + n_elmts + node] = (val0 < val1) ? tree_idx[offset + 2*node +0]:
			                                                    tree_idx[offset + 2*node +1];

			offset += n_elmts;
		}

		// on obtient le deuxime min (ou max)
		result.second = tree_idx[tree_idx.size() -1];

		return result;
	}
};

#endif /* LC_SORTER_HPP */
