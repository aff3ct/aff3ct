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
}
}

#endif /* GENERAL_UTILS_H_*/
