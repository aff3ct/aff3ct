#include <algorithm>
#include <cassert>
#include <sstream>
#include <iomanip>

#include "Tools/general_utils.h"

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
	mutual_sort(vec_abscissa, vec_ordinate, [](const Ta& a, const Ta& b){ return a < b; });
}

template <typename Ta, typename To>
void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate)
{
	mutual_sort(vec_abscissa, vec_ordinate, [](const Ta& a, const Ta& b){ return a < b; });
}

template <typename Ta, typename To, class Compare>
void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<To>& vec_ordinate, Compare comp)
{
	assert(vec_abscissa.size() == vec_ordinate.size());

	for (unsigned i = 1; i < vec_abscissa.size(); i++)
		for (unsigned j = i; j > 0 && comp(vec_abscissa[j], vec_abscissa[j-1]); j--)
		{
			std::swap(vec_abscissa[j], vec_abscissa[j-1]); // order the x position
			std::swap(vec_ordinate[j], vec_ordinate[j-1]); // the y follow their x, moving the same way
		}
}

template <typename Ta, typename To, class Compare>
void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate, Compare comp)
{
#ifndef NDEBUG
	bool good = true;
	for (unsigned k = 0; k < vec_ordinate.size(); k++)
		good &= vec_abscissa.size() == vec_ordinate[k].size();
	assert(good);
#endif

	for (unsigned i = 1; i < vec_abscissa.size(); i++)
		for (unsigned j = i; j > 0 && comp(vec_abscissa[j], vec_abscissa[j-1]); j--)
		{
			std::swap(vec_abscissa[j], vec_abscissa[j-1]); // order the x position

			for (unsigned k = 0; k < vec_ordinate.size(); k++)
				std::swap(vec_ordinate[k][j], vec_ordinate[k][j-1]); // the y follow their x, moving the same way
		}
}

template <typename Ta, typename To>
void mutual_unique(std::vector<Ta>& vec_abscissa, std::vector<To>& vec_ordinate)
{
	assert(vec_abscissa.size() == vec_ordinate.size());

	unsigned r = 0;

	for (unsigned i = 1; i < vec_abscissa.size(); i++)
	{
		if ((vec_abscissa[r] != vec_abscissa[i]) && (++r != i))
		{
			vec_abscissa[r] = std::move(vec_abscissa[i]);
			vec_ordinate[r] = std::move(vec_ordinate[i]);
		}
	}

	if (vec_abscissa.size() != 0 && r == 0)
		r = 1;

	vec_abscissa.resize(r);
	vec_ordinate.resize(r);
}

template <typename Ta, typename To>
void mutual_unique(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate)
{
#ifndef NDEBUG
	bool good = true;
	for (unsigned k = 0; k < vec_ordinate.size(); k++)
		good &= vec_abscissa.size() == vec_ordinate[k].size();
	assert(good);
#endif

	unsigned r = 0;

	for (unsigned i = 1; i < vec_abscissa.size(); i++)
	{
		if ((vec_abscissa[r] != vec_abscissa[i]) && (++r != i))
		{
			vec_abscissa[r] = std::move(vec_abscissa[i]);
			for (unsigned k = 0; k < vec_ordinate.size(); k++)
				vec_ordinate[k][r] = std::move(vec_ordinate[k][i]);
		}
	}

	if (vec_abscissa.size() != 0 && r == 0)
		r = 1;

	vec_abscissa.resize(r);
	vec_ordinate.resize(r);
}


template <typename T>
std::string get_time_format(T secondes)
{
	auto ss = (int)secondes % 60;
	auto mm = ((int)(secondes / 60.f) % 60);
	auto hh = (int)(secondes / 3600.f);

	std::stringstream time_format;

	time_format << std::setw(2) << std::setfill('0') << hh << "h";
	time_format << std::setw(2) << std::setfill('0') << mm << "'";
	time_format << std::setw(2) << std::setfill('0') << ss;

	return time_format.str();
}

template<typename T, typename U>
constexpr size_t offsetOf(U T::*member)
{
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

template<typename T>
std::vector<T*> convert_to_ptr(const std::vector<std::unique_ptr<T>> &v)
{
	std::vector<T*> v2;
	for (auto &e : v)
		v2.push_back(e.get());
	return v2;
}

template<typename T>
std::vector<T*> convert_to_ptr(const std::vector<std::shared_ptr<T>> &v)
{
	std::vector<T*> v2;
	for (auto &e : v)
		v2.push_back(e.get());
	return v2;
}

}
}
