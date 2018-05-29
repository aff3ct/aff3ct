#ifndef GENERAL_UTILS_HXX
#define GENERAL_UTILS_HXX

#include <algorithm>
#include "general_utils.h"

namespace aff3ct
{
namespace tools
{

template <typename BidirectionalIterator, typename T>
BidirectionalIterator get_closest(BidirectionalIterator first, BidirectionalIterator last, const T & value)
{ // https://stackoverflow.com/a/701141/7219905

	auto before = std::lower_bound(first, last, value);

	if (before == first) return first;
	if (before == last)  return --last; // iterator must be bidirectional

	auto after = before;
	--before;

	return (*after - value) < (value - *before) ? after : before;
}

template <typename BidirectionalIterator, typename T>
std::size_t get_closest_index(BidirectionalIterator first, BidirectionalIterator last, const T & value)
{
	return std::distance(first, get_closest(first, last, value));
}

template <typename Ta, typename To>
void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<To>& vec_ordinate)
{
	for (unsigned i = 1; i < vec_abscissa.size(); i++)
		for (unsigned j = i; j > 0 && vec_abscissa[j] < vec_abscissa[j-1]; j--)
		{
			std::swap(vec_abscissa[j], vec_abscissa[j-1]); // order the x position
			std::swap(vec_ordinate[j], vec_ordinate[j-1]); // the y follow their x, moving the same way
		}
}

template <typename Ta, typename To>
void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate)
{
	for (unsigned i = 1; i < vec_abscissa.size(); i++)
		for (unsigned j = i; j > 0 && vec_abscissa[j] < vec_abscissa[j-1]; j--)
		{
			std::swap(vec_abscissa[j], vec_abscissa[j-1]); // order the x position

			for (unsigned k = 0; k < vec_ordinate.size(); k++)
				std::swap(vec_ordinate[k][j], vec_ordinate[k][j-1]); // the y follow their x, moving the same way
		}
}

}
}

#endif //GENERAL_UTILS_HXX
