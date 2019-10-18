#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 * \file
 * \brief Functions for SIMD transpositions.
 */
#ifndef TRANSPOSE_SELECTOR_H
#define	TRANSPOSE_SELECTOR_H

namespace aff3ct
{
namespace tools
{
bool char_transpose (const signed char *src, signed char *dst, int n);
bool char_itranspose(const signed char *src, signed char *dst, int n);
}
}

#endif /* TRANSPOSE_SELECTOR_H */

#endif
