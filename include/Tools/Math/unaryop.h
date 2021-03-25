#ifndef UNARYOP_H
#define UNARYOP_H

namespace aff3ct
{
namespace tools
{
// -------------------------------------------------------------------------- special function prototypes for templates

template <typename TI, typename TO>
using proto_uop = TO (*)(const TI a);

// ------------------------------------------------------------------------------------------- special function headers

template <typename TI, typename TO> inline TO uop_abs    (const TI a);
template <typename TI, typename TO> inline TO uop_not    (const TI a);
template <typename TI, typename TO> inline TO uop_not_abs(const TI a);
template <typename TI, typename TO> inline TO uop_sign   (const TI a);

template <typename TI, typename TO, proto_uop<TI,TO> UOP>
std::string uop_get_name();

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Math/unaryop.hxx"
#endif

#endif /* UNARYOP_H */
