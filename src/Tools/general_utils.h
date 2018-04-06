#ifndef GENERAL_UTILS_H_
#define GENERAL_UTILS_H_

#include <vector>
#include <string>
#include <sstream>

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
 * The second is between 2 or 3. The first element is the min value, the last is the max value. The one between if any
 * is the step to use else use the step 'default_step'.
 */
template <typename R = float>
std::vector<R> generate_range(const std::vector<std::vector<R>>& range_description, const R default_step = (R)0.1);
}
}

#endif /* GENERAL_UTILS_H_*/
