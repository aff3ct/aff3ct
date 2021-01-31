/*!
 * \file
 * \brief Class tools::Interface_get_set_frozen_bits.
 */
#ifndef fb_assert_h__
#define fb_assert_h__

#include <vector>

namespace aff3ct
{
namespace tools
{
inline void fb_assert(const std::vector<bool>& frozen_bits, const int K, const int N);
}
}

#include "Tools/Code/Polar/fb_assert.hxx"
#endif // fb_assert_h__