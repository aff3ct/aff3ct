/*!
 * \file
 * \brief Functions for general operations.
 */
#ifndef GENERAL_UTILS_H_
#define GENERAL_UTILS_H_

#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <typeindex>
#include <memory>

namespace aff3ct
{
namespace tools
{
std::vector<std::string> split(const std::string &s, char delim);

std::vector<std::string> split(const std::string &s);

void getline(std::istream &file, std::string &line);

template <typename R = float>
R sigma_to_esn0(const R sigma, const int upsample_factor = 1);

template <typename R = float>
R esn0_to_sigma(const R esn0, const int upsample_factor = 1);

template <typename R = float>
R esn0_to_ebn0(const R esn0, const R bit_rate = 1, const int bps = 1);

template <typename R = float>
R ebn0_to_esn0(const R ebn0, const R bit_rate = 1, const int bps = 1);

/* Transform a Matlab style description of a range into a vector.
 * For ex: "0:1.2,3.4:0.2:4.4" gives the vector "0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1 1.1 1.2 3.4 3.6 3.8 4 4.2 4.4"
 * The first dimension of 'range_description' can have any size.
 * The second is between 1 to 3. If 1, then a value alone, else the first element is the min value, the last is the max value. The one between if any
 * is the step to use else use the step 'default_step'.
 */
template <typename R = float>
std::vector<R> generate_range(const std::vector<std::vector<R>>& range_description, const R default_step = (R)0.1);

/*
 * Get the nearest position of value in range [first, last[.
 * the reference data must be sorted and strictly monotonic increasing
 * If value goes out of x_data range, then return the left or right limit value in function of the violated one.
 */
template <typename BidirectionalIterator, typename T>
inline BidirectionalIterator get_closest(BidirectionalIterator first, BidirectionalIterator last, const T & value);

template <typename BidirectionalIterator, typename T>
inline std::size_t get_closest_index(BidirectionalIterator first, BidirectionalIterator last, const T & value);

/*
 * Sort 'vec_abscissa' in ascending order and move the matching ordinate of 'vec_ordinate' by the same time.
 */
template <typename Ta, typename To>
inline void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<To>& vec_ordinate);

template <typename Ta, typename To>
inline void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate);

/*
 * Sort 'vec_abscissa' in function of the comp function and move the matching ordinate of 'vec_ordinate' by the same time.
 */
template <typename Ta, typename To, class Compare>
inline void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<To>& vec_ordinate, Compare comp);

template <typename Ta, typename To, class Compare>
inline void mutual_sort(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate, Compare comp);

/*
 * Eliminates all but the first element from every consecutive group of equivalent elements from the 'vec_abscissa' vector
 * and remove the matching ordinate of 'vec_ordinate' by the same time.
 * 'vec_abscissa' and 'vec_ordinate' are resized to their new length.
 */
template <typename Ta, typename To>
inline void mutual_unique(std::vector<Ta>& vec_abscissa, std::vector<To>& vec_ordinate);

template <typename Ta, typename To>
inline void mutual_unique(std::vector<Ta>& vec_abscissa, std::vector<std::vector<To>>& vec_ordinate);

/*
 * Convert the number of seconds into a __h__'__ string format
 * For exemple 3662 secondes would be displayed as "01h01'02".
 * "secondes" is first converted into a "int" type to be processed
 */
template <typename T>
std::string get_time_format(T secondes);

/*
 * calculate offset of a class member at compile time
 * Source : https://stackoverflow.com/a/20141143/7219905
 */
template<typename T, typename U>
constexpr size_t offsetOf(U T::*member);

template <typename T>
std::vector<T*> convert_to_ptr(const std::vector<std::unique_ptr<T>> &v);

template <typename T>
std::vector<T*> convert_to_ptr(const std::vector<std::shared_ptr<T>> &v);

template <typename T>
void check_LUT(const std::vector<T> &LUT, const std::string &LUT_name = "LUT", const size_t LUT_size = 0);

size_t compute_bytes(const size_t n_elmts, const std::type_index type);

std::vector<size_t> compute_bytes(const std::vector<size_t> &n_elmts, const std::vector<std::type_index> &type);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/general_utils.hxx"
#endif

#endif /* GENERAL_UTILS_H_*/
