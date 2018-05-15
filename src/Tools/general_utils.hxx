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

}
}

#endif //GENERAL_UTILS_HXX
