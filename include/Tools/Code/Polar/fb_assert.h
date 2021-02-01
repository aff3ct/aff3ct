#ifndef FB_ASSERT_H
#define FB_ASSERT_H

#include <vector>

namespace aff3ct
{
namespace tools
{
void fb_assert(const std::vector<bool>& frozen_bits, const int K, const int N);
}
}

#endif // FB_ASSERT_H