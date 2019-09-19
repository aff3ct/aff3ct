#ifndef FB_EXTRACT_HPP
#define FB_EXTRACT_HPP

#include <vector>
#include <utility>

namespace aff3ct
{
namespace tools
{
template <typename B, int N_FRAMES = 1>
inline void fb_extract(const std::vector<std::pair<unsigned char, int>> &leaves_patterns, const B *V_N, B *V_K);
}
}

#include "Tools/Code/Polar/fb_extract.hxx"

#endif /* FB_EXTRACT_HPP */
