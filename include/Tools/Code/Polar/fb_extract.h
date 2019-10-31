#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Function to extract the information bit from a Polar codeword.
 */
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/Polar/fb_extract.hxx"
#endif

#endif /* FB_EXTRACT_HPP */

#endif
